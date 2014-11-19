#include <stdio.h>
#include <sys/time.h>
#include "../svcclient/ZPClient.h"
#include "../include/BinderBase.h"

using namespace android;

void get_current_time();
void c_test_callback(int result, void *userdata);


int main(int argc, char** argv)
{
	ZPClient client;
	get_current_time();
	int ret = client.test_num(2014);
	printf("test_num return: %d\n", ret);
	ret = client.test_callback(c_test_callback);
	printf("test_cb. %d\n", ret);
	get_current_time();
	return 0;
}

void get_current_time()
{
	struct timeval ttval;
	int nRet = gettimeofday(&ttval, NULL);
	if(nRet == 0)
	{
		printf("gettimeofday ok. cur = %d:%d \n", ttval.tv_sec, ttval.tv_usec);
	}
}

void c_test_callback(int result, void *userdata)
{
	printf("\n==================================================\n");
	printf("                Test DONE Result :%d", result);
	printf("\n==================================================\n");
}






