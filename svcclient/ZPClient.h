#ifndef ANDROID_ZPCLIENT_H
#define ANDROID_ZPCLIENT_H

typedef void (*test_callback)(int result);

namespace android
{
	class ZPClient
	{
	public:
		int test_num(int n);
		void test_cb();
	private:
		static void getZPService();
		static int set_num(int num);
		static void add_cb(test_callback cb);
		static void do_something();
	};
}
#endif