///服务器端
#include "head.h"
#include "err.h"

///登录验证函数
void login_test(int conn_fd)
{
	FILE *fp;
        char recv_buf[LEN];
	char username[LEN], passwd[LEN];
	int sock_fd;

	///打开保存有用户登录信息的文件
        if((fp = fopen("install", "r")) == NULL) {
                printf("抱歉，系统出现故障!\n");
                my_err("fopen", __LINE__);
        }
	
	///若用户信息输入不成功，循环输入
	while(1) {

		memset(recv_buf, 0, sizeof(recv_buf));		

		///接收输入的用户名
       		if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                	my_err("recv", __LINE__);
	 	strcpy(username, recv_buf);
		memset(recv_buf, 0, sizeof(recv_buf));

		///接收输入的密码
		if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                	my_err("recv", __LINE__);
        	strcpy(passwd, recv_buf);
		memset(recv_buf, 0, sizeof(recv_buf));

		///从文件中读取用户信息并比较
        	while(fscanf(fp, "%s\n", user_msg.username) != EOF) {
			fscanf(fp, "%s\n", user_msg.passwd);
			if((strcmp(username, user_msg.username) == 0) && (strcmp(passwd, user_msg.passwd) == 0)) {
				if(send(conn_fd, "y", 1, 0) < 0) 
					my_err("send", __LINE__);
				if(recv(conn_fd, &sock_fd, sizeof(int), 0) < 0)
					my_err("recv", __LINE__);
				printf("%d\n", sock_fd);
				return ;
			}
				
		}
		if(send(conn_fd, "n", 1, 0) < 0)
			my_err("send", __LINE__);
		
	}
}


///c/s交流函数
 void *communicate(int conn_fd)
 {
         login_test(conn_fd);
 }

int main()
{  
        int sock_fd, conn_fd;
        int optval;
        int ret;
        pthread_t tid;
        socklen_t cli_len;
        struct sockaddr_in cli_addr, serv_addr;

        ///创建一个tcp套接字
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(sock_fd < 0)
                my_err("socket", __LINE__);

        ///设置该套接字使之可以重新绑定端口
        optval = 1;
        if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
   		my_err("setsockopt", __LINE__);

        ///初始化服务器端地址结构
        memset(&serv_addr, 0, sizeof(struct sockaddr_in));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(SERV_PORT);
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        ///将套接字绑定到本地端口
        if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
                my_err("bind", __LINE__);

        ///将套接字转化为监听套接字
        if(listen(sock_fd, LISTENQ) < 0)
                my_err("listen", __LINE__);

	cli_len = sizeof(struct sockaddr_in);
	while (1) {
                ///通过accept接收客户端的连接请求，并返回连接套接字用于收发数据
                conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
                if(conn_fd < 0)
                        my_err("accept", __LINE__);
		///客户端服务端分别提示连接成功
                printf("accept a new client, ip: %s\n", inet_ntoa(cli_addr.sin_addr));

                ///创建一个子线程处理刚刚接收的连接请求
		if(pthread_create(&tid, NULL, communicate, conn_fd) != 0) {
			my_err("pthread_create", __LINE__);
			close(conn_fd);
		}
	}
}
