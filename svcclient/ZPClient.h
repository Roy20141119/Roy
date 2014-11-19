#ifndef ANDROID_ZPCLIENT_H
#define ANDROID_ZPCLIENT_H

#include <binder/IServiceManager.h>
#include "../include/BinderBase.h"

namespace android
{
	class ZPClient
	{
	public:
		int test_num(int n);
		int test_callback(do_something_callback cb);
	private:
		static void addService(sp<IBinder> &binder, const char* key);
		static sp<IBinder> getService(const char* key);
		static int set_num(int num);
		static int add_cb(do_something_callback cb);
		static int do_something();
	};
}
#endif