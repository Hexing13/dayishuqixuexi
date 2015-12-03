/*************************************************************************
	> File Name: 5.c
	> Author: 
	> Mail: 
	> Created Time: 2015年10月21日 星期三 20时23分43秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<pthread.h>
#include<semaphore.h>

#define LOOP_TIMES 10000
sem_t s1,s2;
void* thread_worker(void*);
void critical_section(int thread_num,int i);

int main(void)
{
    int rtn,i;
    sem_init(&s1,0,1);
    sem_init(&s2,0,1);
    pthread_t pthread_id = 0;
    rtn = pthread_create(&pthread_id,NULL,thread_worker,NULL);
    if(rtn!=0){
        printf("pthread_create ERROR!\n");
        return -1;
    }
    for(i = 0;i < LOOP_TIMES;i++){
        sem_wait(&s1);
        sem_wait(&s2);
        critical_section(1,i);
        sem_post(&s1);
        sem_post(&s2);
        sem_destroy(&s1);
        sem_destroy(&s2);
    }
    return 0;
}
void* thread_worker(void*p){
    int i;
    for(i = 0; i < LOOP_TIMES;i++){
        sem_wait(&s1);
        sem_wait(&s2);
        critical_section(2,i);
        sem_post(&s1);
        sem_post(&s2);
    }
}

void critical_section(int thread_num,int i){
    printf("Thread%d:%d\n",thread_num,i);
}
