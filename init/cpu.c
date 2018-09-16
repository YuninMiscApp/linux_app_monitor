#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <time.h>
#include <unistd.h>
#include <assert.h>   

#define __DEBUG__ 0   
#define CK_TIME 1   
  
int main(int argc ,char *argv[])  
{  
    FILE *fp;  
    char buf[128];  
    char cpu[8];  
    unsigned long int user,nice,sys,idle,iowait,irq,softirq,steal;  
    unsigned long int all1,all2,idle1,idle2;
    float cpu_usage;  
	char *pResult = NULL;
    while(1)  
    {  
        if((fp = fopen("/proc/stat","r")) == NULL)  
        {  
            perror("fopen /proc/stat:");  
            exit (0);  
        }  

        pResult = fgets(buf,sizeof(buf),fp);
        assert(pResult);
        sscanf(buf,"%s%lu%lu%lu%lu%lu%lu%lu%lu",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq,&steal);  
  
#if __DEBUG__ 
        printf("buf=%s",buf);
        printf("%s,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",cpu,user,nice,sys,idle,iowait,irq,softirq,steal); 
#endif 
        all1 = user+nice+sys+idle+iowait+irq+softirq+steal;
        idle1 = idle; 
        
        rewind(fp);  
        sleep(CK_TIME);  
        memset(buf,0,sizeof(buf));  
        cpu[0] = '\0';  
        user=nice=sys=idle=iowait=irq=softirq=steal=0;
        pResult = fgets(buf,sizeof(buf),fp);  
        assert(pResult);
        sscanf(buf,"%s%lu%lu%lu%lu%lu%lu%lu%lu",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq,&steal);  
#if __DEBUG__ 
        printf("buf=%s",buf);  
        printf("%s,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",cpu,user,nice,sys,idle,iowait,irq,softirq,steal); 
#endif 

        all2 = user+nice+sys+idle+iowait+irq+softirq+steal;
        idle2 = idle;
        cpu_usage = (all2-all1-(idle2-idle1))*100.0 / (all2-all1) ;  
        fclose(fp);  

        printf("#### cpu use =%6.2f%% ####\n", cpu_usage);
    }  
    return 0;  
}

