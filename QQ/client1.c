#include "head.h"
#include "err.h"

int sock_fd;

//不回显函数
int getch(void)
{
     struct termios tm, tm_old;
     int fd = 0, ch;
	 
     if (tcgetattr(fd, &tm) < 0) {		//获取终端信息
	  return -1;
     }
	 
     tm_old = tm;
     cfmakeraw(&tm);				//将终端设置为原始模式，原始模式下终端不回显
     if (tcsetattr(fd, TCSANOW, &tm) < 0) {	
	  return -1;
     }
	 
     ch = fgetc(stdin);
     if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {	//设置回开始模式
	  return -1;
     }
	    
     return ch;					//返回输入的字符
}

///登录验证函数
void login()
{
	char username[LEN], passwd[LEN];
	char recv_buf[BUFSIZE], str;
	int  i = 0;
	
	while(1) {
		printf("\n请输入用户名:");
		scanf("%s", username);
		if(send(sock_fd, username, sizeof(username), 0) < 0)
			my_err("send", __LINE__);
		printf("\n请输入用户密码:");
		getchar();
		while(i < 7) {
			str = getch();
			if((int)str == 127 && i == 0);		//第一个就输入退格键
			else if((int)str == 127 && i != 0) {	//非第一个输入退格键
				i--;
				printf("\b \b");
			}
			else if((int)str == 13)	 {		//输入回车键
				passwd[i] == '\0';
				break;
			}
			else {					//正常情况
				passwd[i++] = str;
				printf("*");
			}		
		}	
		if(send(sock_fd, passwd, sizeof(passwd), 0) < 0) 
			my_err("send", __LINE__);
		memset(recv_buf, 0, sizeof(recv_buf));
		if(recv(sock_fd, recv_buf, sizeof(recv_buf), 0) < 0)
			my_err("recv", __LINE__);
		if(strcmp(recv_buf, "y") == 0) {
			printf("\n登录成功!\n");
			if(send(sock_fd, &sock_fd, sizeof(int), 0) < 0)
				my_err("send", __LINE__);
			printf("%d\n", sock_fd);
			return ;
		}else{
			printf("\n用户名或密码输入错误!\n");
		}
	}
	
}

int main(int argc, char **argv)
{       
	int i;
        int ret;
        int serv_port;
        struct sockaddr_in serv_addr;
        char recv_buf[BUFSIZE];
        char input_buf[LEN];

        //检查参数个数
        if(argc != 5) {
                printf("usage: [-p] [serv_port] [-a] [serv_address]\n");
                exit(1);
        }

        //初始化服务器端地址结构
        memset(&serv_addr, 0, sizeof(struct sockaddr_in));
        serv_addr.sin_family = AF_INET;

        //从命令行获取服务器端的端口和地址
        for(i = 1; i < argc; i++) {
                if(strcmp("-p", argv[i]) == 0) {
                        serv_port = atoi(argv[i+1]);
                        if(serv_port < 0 || serv_port > 65535) {
                                printf("invalid serv_addr.sin_port\n");
                                exit(1);
                        } else {
                                serv_addr.sin_port = htons(serv_port);
                        }

                        continue;
                }

                if(strcmp("-a", argv[i]) == 0) {
	        	if(inet_aton(argv[i+1], &serv_addr.sin_addr) == 0) {
                                printf("invalid server ip address\n");
                                exit(1);
                	}
              	        continue;
                }
        }

        //检查是否少输入了某项参数
        if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr == 0) {
                printf("usage: [-p] [serv_addr.sin_port] [-a] [serv_address]\n");               exit(1);
        }

        //创建一个tcp套接字
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(sock_fd < 0)
                my_err("socket", __LINE__);
	printf("%d\n", sock_fd);
        //向服务器端发送连接请求
        if(connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
                my_err("connect", __LINE__);
	login();
	return 0;
}
