#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <time.h>
#include <unistd.h>
#include <assert.h>   

#define CK_TIME 1   
  
int main(int argc ,char *argv[])  
{  
    FILE *fp;  
    char buf[128], mem_type[24], kb[8];  
    unsigned long int mem_tot, mem_free, mem_buffer, mem_cache, mem_real_free;
    float mem_usage;  
	char *pResult = NULL;
    while(1)  
    {  
        if((fp = fopen ("/proc/meminfo", "r")) == NULL)  
        {  
            perror("fopen /proc/stat:");  
            exit (0);  
        }

        //MemTotal
        pResult = fgets (buf, sizeof(buf), fp);
        assert(pResult);
        sscanf (buf, "%s %lu %s", mem_type, &mem_tot, kb);
        //MemFree
        pResult = fgets (buf, sizeof(buf), fp);
        assert(pResult);
        sscanf (buf, "%s %lu %s", mem_type, &mem_free, kb);
        //MemAvailable , unuse
        pResult = fgets (buf, sizeof(buf), fp);
        assert(pResult);
        //Buffers
        pResult = fgets (buf, sizeof(buf), fp); 
        assert(pResult);
        sscanf (buf, "%s %lu %s", mem_type, &mem_buffer, kb);
        //Cached
        pResult = fgets (buf, sizeof(buf), fp); 
        assert(pResult);
        sscanf (buf, "%s %lu %s", mem_type, &mem_cache, kb);
        fclose(fp);

        mem_real_free = mem_free + mem_buffer + mem_cache;
        mem_usage = (mem_tot - mem_real_free)*100.0/mem_tot;
        printf("#### mem use =%6.2f%%, mem free = %lu kB ####\n", mem_usage, mem_real_free);
        
        sleep(CK_TIME);  
    }  
    return 0;  
}

