/*************************************************************************
	> File Name: open.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月22日 星期三 14时46分47秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

int main()
{
    int fd;
    char *str;
    if((fd = open("1.c",O_CREAT,S_IRUSR|S_IWUSR))!=-1){
        printf("open:%s with error:%d\n",strerror(errno),errno);
        exit(-1);
    }else{
        printf("create file success\n");
    }
    close(fd);
    return 0;
}
