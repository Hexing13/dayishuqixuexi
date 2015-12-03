/*************************************************************************
	> File Name: pipe.c
	> Author: 
	> Mail: 
	> Created Time: 2015年11月04日 星期三 19时38分18秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>


//读管道
void read_from_pipe(int fd){
    char message[100];
    read(fd,message,100);
    printf("read from pipe: \n%s",message);
    printf("\n");
}

//写管道
void write_to_pipe(int fd){
    char *message = "hello,pipe!\n";
    write(fd,message,strlen(message));
}

int main()
{
    int fd[2];
    pid_t pid;

    //创建管道
    if(pipe(fd)){
        perror("create pipe failed!");
        exit(1);
    }
    //创建子进程
    if(!(pid = fork())){
        close(fd[0]);
        while(1){
            sleep(1);
            write_to_pipe(fd[1]);
        }
    }else if(pid < 0){
        perror("create child failed");
    }else{
        close(fd[1]);
        while(1){
            sleep(3);
            read_from_pipe(fd[0]);
        }
    }
    return 0;
}

