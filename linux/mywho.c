#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<utmp.h>
#include<time.h>
#include</usr/include/asm-generic/fcntl.h>

int main(int argc,char **argv)
{
	int utmp_dp;
	struct utmp buf;
	struct tm *local;

	utmp_dp = open("/var/run/utmp",O_RDONLY);
	if(utmp_dp == -1){
		perror("file");
		exit(EXIT_FAILURE);
	}

	while(read(utmp_dp,&buf,sizeof(struct utmp))){
		if(buf.ut_type == USER_PROCESS){
			printf("%s\t %s",buf.ut_user,buf.ut_line);
			local=localtime(&(buf.ut_tv.tv_sec));
			printf("\t\t%d-%d-%d ",local->tm_year+1900,local->tm_mon+1,local->tm_mday);
			printf("%d:%d ",local->tm_hour,local->tm_min);
			printf("(%s)\n",buf.ut_host);
		}
	}	
	return 0;
}
