#include <stdio.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include "ZPClient.h"

namespace android
{
	sp<IBinder> binder;
	
	//public
	
	int ZPClient::test_num(int n)
	{
		getZPService();
		if(binder == 0)
		{
			printf("ZPService not published, waiting... \n");
			return 0;
		}
		printf("client - getService2:\n");
		
		return set_num(n);
	}
	
	void ZPClient::test_cb()
	{
		getZPService();
		if(binder == 0)
		{
			printf("ZPService not published, waiting... \n");
			return;
		}
		printf("client - getService2:\n");
		
		//add_cb();
		do_something();
	}
	
	//private

	void ZPClient::getZPService()
	{
		printf("client - getService1: \n");
		sp<IServiceManager> sm = defaultServiceManager();
		printf("client - ServiceManager: %p\n", sm.get());
		binder = sm->getService(String16("roy.test"));
	}
	
	int ZPClient::set_num(int n)
	{
		Parcel data, reply;
		data.writeInt32(getpid());
		data.writeInt32(n);
		binder->transact(0, data, &reply);
		int r = reply.readInt32();
		return r;
	}
	
	void ZPClient::add_cb(test_callback cb)
	{
		
	}
	
	void ZPClient::do_something()
	{
		
	}
}