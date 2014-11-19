#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include "ZPService.h"
#include <stdio.h>
#include "../include/BinderBase.h"

typedef void (*do_callback)(int result);

namespace android
{
	static pthread_key_t sigbuskey;
	do_callback callback = NULL;
	pthread_t m_threadIdTimer = NULL;
	char client_key[128];

	void do_something(int arg);
	void do_cb(int cb,int arg);
	sp<IBinder> getService(const char* key);
	
	int ZPService::Instance()
	{
		printf("ZPService Instantiate.\n");
		int ret = defaultServiceManager()->addService(String16("server.test"), new ZPService());
		printf("ZPService ret = %d\n", ret);
		return ret;
	}

	ZPService::ZPService()
	{
		printf("ZPService create\n");
		pthread_key_create(&sigbuskey,NULL);
	}

	ZPService::~ZPService()
	{
		pthread_key_delete(sigbuskey);
		printf("ZPService destory\n");
	}

	status_t ZPService::onTransact(uint32_t code, 
								   const Parcel& data, 
								   Parcel* reply,
								   uint32_t flags)
	{
		printf("server - code = %d \n", code);
		switch(code)
		{
		case TEST_NUMBER_TRANSACTION: //num
			{
				pid_t pid = data.readInt32();
				int num = data.readInt32();
				num += 1000;
				reply->writeInt32(num);
				return NO_ERROR;
			} 
		case TEST_ADD_CALLBACK_TRANSACTION: //add cb
			{
				int ret = 1;
				pid_t pid = data.readInt32();
				callback = (do_callback)data.readInt32();
				
				//binder = data.readStrongBinder();
				
				int key_len = data.readInt32(); //read length, only 32bits length support yet                
				printf("\nServer - key_len : %d \n", key_len);           
				if(key_len > 0) {                    
					Parcel::ReadableBlob rblob;                    
					data.readBlob(key_len, &rblob);  
					memset(client_key, 0x0, sizeof(client_key));
					memcpy(client_key, rblob.data(), key_len);
				}

				printf("\nServer - client_key : %s \n", client_key);  
				
				reply->writeInt32(ret);
				return NO_ERROR;
			} 
		case TEST_DO_SOMETHING_TRANSACTION: //do something
			{
				pid_t pid = data.readInt32();
				int ret = 1;
				int arg = data.readInt32();
				do_something(arg);
				reply->writeInt32(ret);
				return NO_ERROR;
			} 
		default:
			return BBinder::onTransact(code, data, reply, flags);
		}
	}


	void* do_something_task(void *)
	{	 	
		printf("\nServer - Enter do_something_task\n");
		while(1){
			//get status
			printf("\nServer - get status \n");
			usleep(1000 * 1000 * 5);
			printf("\nServer - DONE !\n");
	
			if(callback != NULL)
			{
				do_cb((int)callback, 1);	
			}
			
			break;
		}
		printf("\nServer - Exit do_something_task\n");
		pthread_exit(NULL);    
		return NULL;
	}

	//
	void do_something(int arg) {
		printf("\nServer - [arg:%d]Do Something ...\n", arg);
		pthread_create(&m_threadIdTimer, NULL, do_something_task, (void*)NULL);
	}

	sp<IBinder> getService(const char* key)
	{
		printf("\nServer - getService: \n");
		sp<IServiceManager> sm = defaultServiceManager();
		printf("\nServer - ServiceManager: %p\n", sm.get());
		return sm->getService(String16(key));
	}

	void do_cb(int cb,int arg)
	{ 
		Parcel data, reply;
		if(client_key == NULL)
		{
			printf("\nServer - client_key is NULL! \n"); 
			return;
		}
		printf("\nServer - client_key:%s \n", client_key);
		sp<IBinder> binder = getService(client_key);
		data.writeInt32(getpid());
    	data.writeInt32(cb);
		data.writeInt32(arg);
		if(binder->isBinderAlive())
		{
    		printf("\nServer - binder->transact() Callback:0x%x Arg:%d\n",cb,arg);  
    		binder->transact(TEST_START_TRANSACTION, data, &reply);
		}
		else
		{
			printf("\nServer - binder Dead ! \n"); 
		}
	}
}