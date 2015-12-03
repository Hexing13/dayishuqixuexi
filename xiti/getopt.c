/*************************************************************************
	> File Name: getopt.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月22日 星期三 21时14分31秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define PARAM_NONE			0
#define PARAM_a 			1
#define PARAM_l				2
#define PARAM_R				4
#define PARAM_t				8

int main(int argc,char **argv)
{
int param = 0;
char opt;  
opterr = 0;         //不显示参数错误信息  

while ((opt = getopt(argc,argv,"alRtS")) != -1) {  
    if (opt == 'a') {  
        param |= PARAM_a;  
    } else if (opt == 'l') {  
        param |= PARAM_l;  
    } else if (opt == 'R') {  
        param |= PARAM_R;  
    }  else if (opt == 't') {  
        param |= PARAM_t;  
    } else {  
        printf("对不起，目前只支持参数R,S,a,t和l.\n");  
        exit(1);  
    }  
}  
    printf("%d\n",param);
    return 0;
}
