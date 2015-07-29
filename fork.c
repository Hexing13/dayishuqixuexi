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
            printf("孩子进程：%d %d  父进程：%d\n",pid,getpid(),getppid());
            break;
        case -1:
            printf("创建进程失败！\n");
            break;
        default:
            printf("父进程：%d %d\n",pid,getpid());
            break;
    }
    return 0;
}
