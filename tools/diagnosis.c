#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <time.h>
#include <unistd.h>
#include <assert.h>   
#include <ctype.h>
#include <pthread.h>

#include "cmd.h"

#define CK_TIME 5   


void* task_strace(void* arg)
{
	char task_info[1024] = {0,};
    int pid = (int)arg;
	// printf("%s,%d: pid: %d\n",__func__,__LINE__,pid);
	//
	CMD_GET_BUF(task_info,"cat /proc/%d/stat | awk '{print $1, $2}' ",pid);
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %s ",task_info);
	//
	CMD_EXEC("sudo strace -c -p %d ",pid);
    return NULL;
}

int main(int argc ,char *argv[])  
{
	char buf[10240]={0};
    int pid = -1;
	int seconds = CK_TIME;
    int ret = -1;
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <PID> <seconds>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	ret = sscanf(argv[1],"%d",&pid);
	assert(1 == ret);
	ret = sscanf(argv[2],"%d",&seconds);
	assert(1 == ret);
	printf("pid=%d, seconds=%d\n",pid,seconds);
	//
	//while(1)
	{
		//
		system("clear");
		printf("=======================================================================================================\n");
		//
		CMD_GET_BUF(buf,"ls /proc/%d/task/ | xargs",pid);

		char *sub = NULL;
		int i = 0;
		sub = strtok(buf," ");
		while(NULL != sub)
		{
			i = 0;
			while(0 != sub[i])
			{
				if(0 == isdigit(sub[i]))
				{
					sub[i] = 0;
					break;
				}
				i++;
			}
			//
			pthread_t mythread;
			pthread_create(&mythread, NULL, task_strace, (void *)atoi(sub));
			sleep(seconds);
			system("sudo kill -s 15 `ps -ef  | grep strace | grep -v grep | cut -c 9-16 | xargs `");
			sleep(1);
			printf("=======================================================================================================\n");
			//
			sub = strtok(NULL, " ");
		}	
		
	}
	
	//
	return 0;
}

