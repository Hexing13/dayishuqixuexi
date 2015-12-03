/*************************************************************************
	> File Name: uid.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月29日 星期三 16时53分47秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>

extern int errno;

int main()
{
    int fd;

    printf("uid study:\n");
    printf("uid:%d euid:%d\n",getuid(),geteuid());

    if((fd = open("test.c",O_RDWR)) == -1){
        printf("失败!\n");
        exit(1);
    }else{
        printf("成功!\n");
    }

    close(fd);
    exit(0);
}
