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
	printf("%s,%d: pid: %d\n",__func__,__LINE__,pid);
	//
	CMD_GET_BUF(task_info,"cat /proc/%d/stat | awk '{print $1, $2}' ",pid);
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %s ",task_info);
	//
	CMD_EXEC("sudo strace -c -p %d ",pid);
    return NULL;
}

int main(int argc ,char *argv[])  
{
    FILE *fp = NULL;
    char buf[1024*4] = {0,};
    char buf2[1024*4] = {0,};
    char *pResult = NULL;
    int pid = -1;
    char cmd[1024] = {0,};
    int ret = -1;
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	ret = sscanf(argv[1],"%d",&pid);
	assert(1 == ret);
	printf("pid: %d\n",pid);
	//
	//while(1)
	{
		//
		system("clear");
		printf("=======================================================================================================\n");
		//
		memset(cmd,0,sizeof(cmd));
	    snprintf(cmd,sizeof(cmd),"ls /proc/%d/task/ | xargs",pid);
	    fp = popen (cmd, "r");
	    assert(fp);
		memset(buf,0,sizeof(buf));
		pResult = fgets (buf, sizeof (buf), fp);
	    assert(pResult);
	    fclose(fp);
	    fp = NULL;
		// printf("%s,%d: buf: %s\n",__func__,__LINE__,buf);

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
			sleep(CK_TIME);
			system("sudo kill -s 15 `ps -ef  | grep strace | grep -v grep | cut -c 9-16 | xargs `");
			//
			sub = strtok(NULL, " ");
		}	
		
	}
	
	//
	return 0;
}

