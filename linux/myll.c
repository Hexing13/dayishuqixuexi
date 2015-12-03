#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<dirent.h>

#include</usr/include/asm-generic/fcntl.h>

void uid_name(int uid,char *name,int flag)//两用函数
{
	FILE *fp;
	char str_uid[5];//UID的字符串格式
	char buf[200];//文件每行的内容
	int i=0;//取出来属主名
	memset(str_uid,'\0',5);
	memset(buf,'\0',200);
	if(flag == 1)	
		fp = fopen("/etc/passwd","r");
	else
		fp = fopen("/etc/group","r");

		if(fp == NULL)
	{
		perror("/etc/passwd");
		exit(EXIT_FAILURE);
	}
	
	//找出含有UID的那一行
	while(fgets(buf,200,fp))
	{
		sprintf(str_uid,"%d",uid);
		if(strstr(buf,str_uid))//找到就退出循环了
		{
			//printf("%s",buf);
			break;
		}		
		memset(buf,'\0',200);
		memset(str_uid,'\0',5);
	}
	while(buf[i]!=':')
	{
		name[i]=buf[i];
		i++;	
	}
	//puts(name);
}

void trans_mode(int mode)
{
	char mode_info[10];//存储状态字符串
	memset(mode_info,' ',10);
	int pos=0;//指示权限位（9bit）
	switch(mode & __S_IFMT)
	{
		case __S_IFDIR:	mode_info[0]='d';break;
		case __S_IFCHR:	mode_info[0]='c';break;
		case __S_IFBLK:	mode_info[0]='b';break;
		case __S_IFREG:	mode_info[0]='-';break;
		case __S_IFIFO:	mode_info[0]='p';break;
		case __S_IFLNK:	mode_info[0]='l';break;
		case __S_IFSOCK:mode_info[0]='s';break;
		default :mode_info[0]='?';break;
	}
	
	while(pos<9)
	{
		if(mode & (int)pow(2,pos))
		{
			if(pos%3 == 0)
				mode_info[9-pos]='x';		
			if(pos%3 == 1)
				mode_info[9-pos]='w';
			if(pos%3 == 2)
				mode_info[9-pos]='r';
		}
		else
			mode_info[9-pos]='-';

		pos++;
	}
	

	while(pos<12)
	{
		if(mode & (int)pow(2,pos))
		{
			if(pos%3 == 0)
				mode_info[9]='t';			
			if(pos%3 == 1)
				mode_info[6]='s';
			if(pos%3 == 2)
				mode_info[3]='s';
		}
		pos++;
	}	

	printf("%s ",mode_info);
}

void _ll(char *argv)
{
	struct stat buf;

	char u_name[20];//属主
	char g_name[20];//所属组
	memset(u_name,'\0',20);
	memset(g_name,'\0',20);

	stat(argv,&buf);

	//处理时间的操作
	struct tm *local;

	
   //   printf("%d ",buf.st_mode);//权限
	trans_mode(buf.st_mode);
	printf("%ld ",buf.st_ino);//inode号
	printf("%d ",buf.st_nlink);//链接数
	//处理UID 和 GID
	//printf("%d %d ",buf.st_uid,buf.st_gid);//用户ID 所属组ID（要重新修改显示为字符名）	
	uid_name(buf.st_uid,u_name,1);
	printf("%s ",u_name);//属主名
	
	uid_name(buf.st_gid,g_name,0);
	printf("%s ",g_name);//所属组名
	
	printf("%ld  ",buf.st_size);//文件大小
	//文件最后的修改时间
	local=localtime(&(buf.st_mtime));
	printf("%d月 %d %d:%d ",local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min);	
	
	printf("%s",argv);//文件名称
	printf("\n");
}

main(int argc,char **argv)
{
	struct stat buf;
	DIR   *dir ; 

/*
	char temp[50];
	memset(temp,'\0',50);	

   	struct dirent  *ptr;

	stat(argv[1],&buf);
	if((buf.st_mode & __S_IFMT) == __S_IFDIR)//目录
	{
		dir=opendir(argv[1]);
		if(dir==NULL)
		{
			perror("dir");
			exit(EXIT_FAILURE);
		}
		while(ptr=readdir(dir))
		{
			puts(ptr->d_name);
			strcpy(temp,ptr->d_name);
			_ll(temp);
			memset(temp,'\0',50);
		}
			
		closedir(dir);
	}
*/

	//else//文件
		_ll(argv[1]);
}
