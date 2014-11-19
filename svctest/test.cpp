#include <stdio.h>
#include <sys/time.h>
#include "../svcclient/ZPClient.h"
#include "TestCB.h"

using namespace android;

void get_current_time();

int main(int argc, char** argv)
{
	ZPClient client;
	printf("test_num return 1111: \n");
	get_current_time();
	int ret = client.test_num(2012);
	printf("test_num return: %d\n", ret);
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
