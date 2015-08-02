///注册函数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///用户名和密码的最大长度
#define LEN 20

///记录用户信息的结构体
struct user_info
{
	char username[LEN];
	char passwd[LEN];
}user_msg;

///自定义的错误处理函数
void my_err(const char *err_string, int line)
{
        fprintf(stderr, "line: %d ", line);
        perror(err_string);
        exit(1);
}

///自定义的注册函数
void install()
{	
	FILE *fp;
	int i;
	if((fp = fopen("install", "a")) == NULL) {
		printf("抱歉，系统出现故障， 不能注册!\n");
		my_err("fopen", __LINE__);
	}
	printf("\n\n********************	欢迎注册	********************");
	printf("\n\n		请输入用户名:");
	scanf("%s", user_msg.username);	
	fprintf(fp, "%s\n", user_msg.username);
	printf("\n\n		请输入密码:");
	scanf("%s", user_msg.passwd);
	fprintf(fp, "%s\n", user_msg.passwd);
	printf("\n\n********************	注册成功	********************");
	printf("\n");
	close(fp);
	
}

int main()
{
	install();
	
	return 0;
}
