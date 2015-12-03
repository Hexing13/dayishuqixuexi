#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include</usr/include/asm-generic/fcntl.h>

int main(int argc, char **argv)
{
	int fp_create;

    while(--argc){
	    fp_create=open(*++argv,O_WRONLY|O_CREAT,0644);
	    if(fp_create==-1){
		    perror("create file");
		    exit(EXIT_FAILURE);
	    }
	    close(fp_create);
    }

    return 0;
}

