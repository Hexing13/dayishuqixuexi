#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include</usr/include/asm-generic/fcntl.h>


int main(int argc,char **argv)
{ 
	int fd;
    while(--argc){
	    fd=open(*++argv,O_RDONLY,0644);
        if(fd<0){
    		perror ( "open"  ); 
	    	exit(EXIT_FAILURE);
	    }
   	    if(unlink(*argv)<0){
    		perror("unlink") ;   
	    	exit(0);
	    }
    }
}

