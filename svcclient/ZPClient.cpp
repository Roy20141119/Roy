#include <stdio.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include "ZPClient.h"
#include "../include/BinderBase.h"

namespace android
{
	const char* client_binder_key = "client.test"; 
	const char* server_binder_key = "server.test"; 
	

	// ClientBinder, Singleton
	
	class ClientBinder : public BBinder
	{
	
	private:
		pthread_key_t sigbuskey;
		static ClientBinder* instance;

		ClientBinder()
		{
			printf("ClientBinder create. \n");
			pthread_key_create(&sigbuskey, NULL);
		}
		
		virtual ~ClientBinder()
		{
			printf("ClientBinder delete. \n");
			pthread_key_delete(sigbuskey);
		}
		
	public:
		static ClientBinder* getInstance()
		{
			printf("instance = %p \n", instance);
			if(instance == NULL)
			{
				instance = new ClientBinder();
			}
			return instance;
		}

		virtual status_t onTransact(uint32_t code , const Parcel &data , Parcel* reply, uint32_t flags)
		{
			printf("ClientBinder onTransact code = %d \n", code);
			switch(code)
			{
			case TEST_START_TRANSACTION:
				{
					int ret = 1;
					pid_t pid = data.readInt32();
					do_something_callback cb = (do_something_callback)data.readInt32();
					if(cb)
					{
						int arg = data.readInt32();
						cb(arg, NULL);
					}
					reply->writeInt32(ret);
					return NO_ERROR;
				}
			default:
				return BBinder::onTransact(code, data, reply, flags);  
			}
		}
		
	};

	ClientBinder* ClientBinder::instance = NULL;

	//public
	
	int ZPClient::test_num(int n)
	{
		printf("client - test_num - \n");
		return set_num(n);
	}
	
	int ZPClient::test_callback(do_something_callback cb)
	{
		printf("client - test_cb - \n");
		sp<ProcessState> proc(ProcessState::self()); 
		add_cb(cb);
		do_something();
		ProcessState::self()->startThreadPool();
		IPCThreadState::self()->joinThreadPool();
		return 0;
	}

	
	//private

	sp<IBinder> ZPClient::getService(const char* key)
	{
		printf("client - getService: \n");
		sp<IServiceManager> sm = defaultServiceManager();
		printf("client - ServiceManager: %p\n", sm.get());
		return sm->getService(String16(key));
	}

	void ZPClient::addService(sp <IBinder> &binder, const char * key)
	{
		printf("client - addService. \n");
		sp<IServiceManager> sm = defaultServiceManager();
		printf("client - ServiceManager: %p\n", sm.get());	
		int ret = sm->addService(String16(key), binder);
		printf("client - addService, ret = %d\n", ret);	
	}
	
	int ZPClient::set_num(int n)
	{
		Parcel data, reply;
		data.writeInt32(getpid());
		data.writeInt32(n);
		sp<IBinder> binder = getService(server_binder_key);
		if(binder == 0)
		{
			printf("binder service is not published, waiting... \n");
			return -1;
		}
		binder->transact(TEST_NUMBER_TRANSACTION, data, &reply);
		int status = reply.readInt32();
		return status;
	}
	
	int ZPClient::add_cb(do_something_callback cb)
	{
		Parcel data, reply; 
		
		sp<IBinder> binder = ClientBinder::getInstance();
		
		if(binder == NULL)
		{
			return -1;
		}

		addService(binder, client_binder_key);
		data.writeInt32(getpid());
        data.writeInt32((int32_t)cb);
		//data.writeStrongBinder(binder);
		printf("add_cb:0x%x \n",cb);
		if (client_binder_key == NULL) {                
			data.writeInt32(-1);           
		}            
		else 
		{    
			int len = sizeof("client.test");
			data.writeInt32((int)len * sizeof(unsigned char));//write length, only support 32 bits length yet             
			Parcel::WritableBlob keyblob;              
			data.writeBlob(len, &keyblob);             
			memcpy(keyblob.data(), client_binder_key, len* sizeof(unsigned char));        
		}
		
		binder = getService(server_binder_key);
		if(binder == 0)
		{
			printf("binder service is not published, waiting... \n");
			return -1;
		}
        binder->transact(TEST_ADD_CALLBACK_TRANSACTION, data, &reply);
		int status = reply.readInt32();
		return status;
	}
	
	int ZPClient::do_something()
	{
		int arg = 443000000;
        Parcel data, reply; 
		data.writeInt32(getpid());
        data.writeInt32(arg);
        printf("do_something arg:%d \n",arg);  
		sp<IBinder> binder = getService(server_binder_key);
		if(binder == 0)
		{
			printf("binder service is not published, waiting... \n");
			return -1;
		}
        binder->transact(TEST_DO_SOMETHING_TRANSACTION, data, &reply);  
        int status = reply.readInt32();
		return status;
	}

}





