#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX 1000
void readcmd(char *cmd);
void do_command(char *cmd);

int main() {
    char cmd[MAX];
    while (1) {
        readcmd(cmd);
        do_command(cmd);
    }
    return 0;
}

void readcmd(char *cmd) {
    if(geteuid()==0){
        printf("myshell#:");
    }else{
        printf("myshell$:");
    }
    gets(cmd);
}

void do_command(char *cmd) {
	//命令名称
    char appName[1000] = {0};
    //参数名称
	char paramList[1000][100] = {0};

	int i = 0;
	//得到命令名称
	for (i = 0;i < sizeof(appName);i++) {
		if (cmd[i] != ' ') {
			appName[i] = cmd[i];
		} else {
			appName[i] = '\0';
			break;
		}
	}

	int j = 0,k = 0;
	//得到命令参数
	for (i = 0;i < strlen(cmd);i++) {
		if (cmd[i] != ' ') {
			paramList[j][k++] = cmd[i];
		} else {
			paramList[j][k] = '\0';
			k = 0;
			j++;
		}
	}

	j++;
	char *param[100];//指向参数的指针数组
	for (i = 0;i < j;i++) {
		param[i] = paramList[i];
	}
	param[i] = NULL;

	int pid = 0;
	if ((pid = vfork()) == -1) {
		printf("进程创建失败.\n");
		exit(1);
	}
	//子进程执行
	if (!pid) {	
		if (execvp(appName,param) == -1) {
			printf("没有此命令!\n");
			return;
		}
    	
	}

	int stat = 0;
	if (waitpid(pid,&stat,0) == -1) {
		printf("等待子进程退出出错\n");
	}
}






















