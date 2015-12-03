/*************************************************************************
	> File Name: fork.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月29日 星期三 10时13分34秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t pid;

    pid = fork();
    switch(pid){
        case 0:
            while(1){
                printf("child:%d parent:%d\n",getpid(),getppid());   
                sleep(3);
            }
        case -1:
            printf("创建进程失败！\n");
            exit(-1);
        default:
            printf("parent：%d\n",getpid());
            exit(0);
    }
    return 0;
}
