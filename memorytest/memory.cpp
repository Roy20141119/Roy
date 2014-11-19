#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <utils/Log.h>

int main(int arg, char** argv)
{
	#define MAX_STEP_SIZE 10*1024*1024
	#define MIN_STEP_SIZE 1*1024*1024
	printf("memory - main() begin. \n");
	void *pMem = NULL;
	
	while(1)
	{
		pMem = malloc(MAX_STEP_SIZE);
		if(pMem)
		{
			memset(pMem, 0x11, MAX_STEP_SIZE);
			printf("memory=%p,size:%d(M)\n", pMem,MAX_STEP_SIZE/1024/1024);
		}
		else
		{
			pMem = malloc(MIN_STEP_SIZE);
			if(pMem)
			{
				memset(pMem, 0x11, MIN_STEP_SIZE);
				printf("memory=%p,size:%d(M)\n", pMem,MIN_STEP_SIZE/1024/1024);
			}
		}
		usleep(3*1000*1000);
	}
	
	printf("memory -  \n");
	return 0;
}