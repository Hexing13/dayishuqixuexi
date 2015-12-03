/*************************************************************************
	> File Name: daemon.c
	> Author: 
	> Mail: 
	> Created Time: 2015年11月04日 星期三 15时57分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

//创建守护进程
void init_daemon(){
    pid_t pid;
    int i;
    if(pid = fork()){
        //是父进程,结束父进程
        exit(0);
    }else if(pid < 0){
        //fork失败,退出
        perror("fork error");
        exit(-1);
    }else{
        //第一个子进程,后台继续执行
        setsid();//第一个子进程成为新的会话组长和进程组长并与控制终端分离
        if(pid = fork()){
            //是第一个子进程,结束
            exit(0);
        } else if(pid < 0){
            perror("fork");
            exit(-1);
        }else{
            //第二个子进程,继续,其不再是会话组长
            for(i = 0; i < getdtablesize();i++){
                //关闭打开的文件描述符
                close(i);
            }
            //重设文件创建掩码
            umask(0);
        }
    }
}

int main()
{
    char *buf = "hello world\n";
    int fd;
    init_daemon();
    while(1){
        if(fd = open("hello.txt",O_CREAT|O_WRONLY|O_APPEND,0600)<0){
            perror("open");
            exit(-1);
        }
        write(fd,buf,strlen(buf));
        close(fd);
        sleep(1);
    }
    return 0;
}
