/*************************************************************************
	> File Name: gdb.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月22日 星期三 09时33分37秒
 ************************************************************************/

#include<stdio.h>
int get_sum(int n)
{
 int sum = 0, i;   
    for(i = 0; i < n;i++){
        sum+=i;
    }
    return sum;
}
int main()
{
    int i = 100,result;
    result = get_sum(i);
    printf("%d\n",result);
    return 0;
}
