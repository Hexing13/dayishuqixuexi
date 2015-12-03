/*************************************************************************
	> File Name: 3.c
	> Author: 
	> Mail: 
	> Created Time: 2015年10月21日 星期三 17时03分28秒
 ************************************************************************/

#include<stdio.h>
#include<ctype.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>

//创建线程的个数
#define MAX_THREAD 3
unsigned long long main_counter,counter[MAX_THREAD];
void* thread_work(void*);

int main(int argc,char* argv[]){
    int i,rtn,ch;
    pthread_t pthread_id[MAX_THREAD]={0};//存放线程id
    for(i = 0; i < MAX_THREAD;i++){
        rtn = pthread_create(&pthread_id[i],NULL,thread_work,(void*)i);
    }

    do{
        unsigned long long sum = 0;
        for(i = 0; i < MAX_THREAD;i++){
            sum+=counter[i];
            printf("pthread NO.%d\n and its counter is %llu\n",i,counter[i]);
        }
        printf("%llu ----- %llu\n",main_counter,sum);
    }while((ch=getchar())!='q');

    return 0;
}

void *thread_work(void* p){
    int thread_num = (int)p;
    for(;;){
        counter[thread_num]++;
        main_counter++;
    }
}
