/*************************************************************************
	> File Name: wait.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月29日 星期三 15时34分36秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main()
{
    pid_t pid;
    char *msg;
    int k;
    int exit_code;

    printf("学习怎样得到code\n");
    pid = fork();
    switch(pid)
    {
        case 0:
            msg = "孩子";
            k = 5;
            exit_code = 37;
            break;
        case -1:
            perror("process creation failed\n");
            exit(1);       
        default:
            exit_code = 0;
            break;
    }

    if(pid != 0){
        int stat_val;
        pid_t child_pid;

        child_pid = wait(&stat_val);
        printf("孩子进程结束,pid = %d\n",child_pid);
        if(WIFEXITED(stat_val)){
            printf("exit_code：%d\n",WIFEXITED(stat_val));
        }else{
            printf("不正常!\n");
        }
    }else{
        while(k-- > 0){
            puts(msg);
            sleep(1);
        }
    }
    exit(exit_code);
}
