#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <termio.h>

///发送消息的最大长度
#define BUFSIZE 100
///用户名和密码的最大长度
#define LEN 20

///服务器端的端口
#define SERV_PORT 4507  
///连接请求队列的最大长度
#define LISTENQ 12

///记录用户信息的结构体
struct user_info
{
        char username[LEN];
        char passwd[LEN];
}user_msg;

///记录在线用户信息的链表
typedef struct online
{       char username[LEN];
        int fd;
        struct online *next;
}ONLINE;


