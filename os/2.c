/*************************************************************************
	> File Name: 2.c
	> Author: 
	> Mail: 
	> Created Time: 2015年10月21日 星期三 15时11分01秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<ctype.h>

//允许建立的子程序的最大个数
#define MAX_CHILD_NUMBER 10
//子进程休眠时间
#define SLEEP_INTERVAL 2
//子进程的自编号 从0开始
int proc_number=0;
void do_something();

int main(int argc,char *argv[]){
    //子进程个数
    int child_proc_number=MAX_CHILD_NUMBER;
    int i,ch;
    pid_t child_pid;
    pid_t pid[10]={0};//存放每个进程的id
    
    //获得要创建的子进程的个数
    if(argc>1){
        child_proc_number=atoi(argv[1]);
        child_proc_number=(child_proc_number>10)?10:child_proc_number;
    }

    for(i = 0;i < child_proc_number;i++){
        child_pid = fork();
        if(child_pid==-1){
            perror("fork");
        }else if(child_pid==0){
            proc_number=i;
            do_something();
        }else{
            pid[i]=child_pid;
        }
    }

    while((ch=getchar())!='q'){
        if(isdigit(ch)){
            kill(pid[ch-'0'],SIGTERM);
            wait(&pid[ch-'0']);
        }
    }

    //杀死本组所有进程
    for(i = 0; i <= proc_number;i++){
        kill(pid[i],SIGTERM);
    }
    return 0;
}

void do_something(){
    for(;;){
        printf("This is process NO.%d %p and its pid is %d\n",proc_number,&proc_number,getppid());
        sleep(SLEEP_INTERVAL);
    }
}
