/*
	> File Name: ls.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月22日 星期三 20时11分54秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>

#define PARAM_NONE 0  //无参数
#define PARAM_a 1  //-a显示所有文件
#define PARAM_l 2  //-l一行只显示一个文件的全部信息
#define MAXROWLEN 100 //每行显示的最多字符数
#define MAX 100 //路径名的最大长度
#define FILE_MAX 300 //目录下最多存储的文件数

int filename_max; //目录下最长文件名的长度
int char_left = MAXROWLEN; //每行的剩余量

int main(int argc,char **argv)
{
    int param = 0; //选项
    int i = 0, num = 0;
    char opt; //getopt()返回值
    char path[300]; //文件或文件夹的名字
    int count = 0;

    //获得选项值
    opterr = 0; //不显示参数错误信息
    while((opt = getopt(argc,argv,"al"))!= -1){
        if(opt == 'a'){
            param |= PARAM_a;
        }else if(opt == 'l'){
            param |= PARAM_l;
            count++;
        }else{
            printf("抱歉，目前只支持al选项查询!\n");
            exit(1);
        }
    }
    if(count==2){
        param = 3;
    }
    printf("%d\n",param);

    //判断是否输入目录或者文件
    for(i = 0; i < argc; i++){
        if(argv[i][0] == '-')
            num++;
    }
    if(num + 1 == argc){
        diaplay(param,getcwd(NULL,0));
        return 0;
    }

    for(i = 1; i < argc; i++){
        if(argv[i][0] != '-'){
            strcpy(path, argv[i]);
            diaplay(param,path);
        }
    }
    return 0;
}


//错误处理函数
void my_err(const char *err_string, int line)
{
    fprintf(stderr,"line: %d ",line);
    perror(err_string);
    exit(1);
}

int str_cmp(const void *a,const void *b)
{
    return strcmp((char*)a,(char*)b);
}

int time_cmp(const void *a,const void *b)
{
    struct stat bufa,bufb;

    if(lstat((char *)a,&bufa)){
        my_err("lstat",__LINE__);
    }
    if(lstat((char *)b,&bufb)){
        my_err("lstat",__LINE__);
    }

    return (bufa.st_mtime < bufb.st_mtime);
}

void display_single(char *name)
{
    int i,len;

    if(char_left < filename_max){
        printf("\n");
        char_left = MAXROWLEN;
    }
    
    len = strlen(name);
    len = filename_max - len;
    printf("%-s",name);
    
    //打印空格
    for(i = 0; i < len; i++)
        printf(" ");
    printf("  ");

    //获得剩余可用字符数
    char_left -= (filename_max+2);
}

void display_attribute(struct stat buf, char *name)
{
    struct passwd *psd; //获取文件所有者的用户名
    struct group *grp;  //获取文件所有者的组名
    char mtime[30];

    if(name[0] == '.'){
        return;
    }
    //获取并打印文件类型
    if (S_ISDIR(buf.st_mode)) {
		printf("d");
	} else if (S_ISLNK(buf.st_mode)) {
		printf("l");
	} else if (S_ISSOCK(buf.st_mode)) {
		printf("s");
	} else if (S_ISFIFO(buf.st_mode)) {
		printf("p");
	} else if (S_ISREG(buf.st_mode)) {
		printf("-");
	} else if (S_ISCHR(buf.st_mode)) {
		printf("c");
	} else if (S_ISBLK(buf.st_mode)) {
		printf("b");
	}

    //所有者权限
    if (S_IRUSR & buf.st_mode) {
		printf("r");
	} else {
		printf("-");
	}
	if (S_IWUSR & buf.st_mode) {
		printf("w");
	} else {
		printf("-");
	}
	if (S_IXUSR & buf.st_mode) {
		printf("x");
	} else {
		printf("-");
	}

	//同组用户权限
    if (S_IRGRP & buf.st_mode) {
		printf("r");
	} else {
		printf("-");
	}
	if (S_IWGRP & buf.st_mode) {
		printf("w");
	} else {
		printf("-");
	}
	if (S_IXGRP & buf.st_mode) {
		printf("x");
	} else {
		printf("-");
	}

    //其他用户权限
	if (S_IROTH & buf.st_mode) {
		printf("r");
	} else {
		printf("-");
	}
	if (S_IWOTH & buf.st_mode) {
		printf("w");
	} else {
		printf("-");
	}
	if (S_IXOTH & buf.st_mode) {
		printf("x");
	} else {
		printf("-");
	}
	printf("  ");
    
    //文件链接数
    printf("%2d",buf.st_nlink);
    printf("  ");

    //根据uid和gid获取文件所有者的用户名和组名
    psd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);
    printf("%8s",psd->pw_name);
    printf("  ");
    printf("%8s",grp->gr_name);
    printf("  ");

    //字节大小
    printf("%8d",buf.st_size);
    printf("  ");

    //修改时间
    strcpy(mtime,ctime(&buf.st_mtime));
    mtime[strlen(mtime)-1] = 0;
    printf("%s",mtime);
    printf("  ");

    printf("%s\n", name);
}

void display_complex(struct stat buf, char *name)
{
    
    struct passwd *psd; //获取文件所有者的用户名
    struct group *grp;  //获取文件所有者的组名
    char mtime[30];

    //获取并打印文件类型
    if (S_ISDIR(buf.st_mode)) {
		printf("d");
	} else if (S_ISLNK(buf.st_mode)) {
		printf("l");
	} else if (S_ISSOCK(buf.st_mode)) {
		printf("s");
	} else if (S_ISFIFO(buf.st_mode)) {
		printf("p");
	} else if (S_ISREG(buf.st_mode)) {
		printf("-");
	} else if (S_ISCHR(buf.st_mode)) {
		printf("c");
	} else if (S_ISBLK(buf.st_mode)) {
		printf("b");
	}

    //所有者权限
    if (S_IRUSR & buf.st_mode) {
		printf("r");
	} else {
		printf("-");
	}
	if (S_IWUSR & buf.st_mode) {
		printf("w");
	} else {
		printf("-");
	}
	if (S_IXUSR & buf.st_mode) {
		printf("x");
	} else {
		printf("-");
	}

	//同组用户权限
    if (S_IRGRP & buf.st_mode) {
		printf("r");
	} else {
		printf("-");
	}
	if (S_IWGRP & buf.st_mode) {
		printf("w");
	} else {
		printf("-");
	}
	if (S_IXGRP & buf.st_mode) {
		printf("x");
	} else {
		printf("-");
	}

    //其他用户权限
	if (S_IROTH & buf.st_mode) {
		printf("r");
	} else {
		printf("-");
	}
	if (S_IWOTH & buf.st_mode) {
		printf("w");
	} else {
		printf("-");
	}
	if (S_IXOTH & buf.st_mode) {
		printf("x");
	} else {
		printf("-");
	}
	printf("  ");
    
    //文件链接数
    printf("%2d",buf.st_nlink);
    printf("  ");

    //根据uid和gid获取文件所有者的用户名和组名
    psd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);
    printf("%8s",psd->pw_name);
    printf("  ");
    printf("%8s",grp->gr_name);
    printf("  ");

    //字节大小
    printf("%8d",buf.st_size);
    printf("  ");

    //修改时间
    strcpy(mtime,ctime(&buf.st_mtime));
    mtime[strlen(mtime)-1] = 0;
    printf("%s",mtime);
    printf("  ");

    printf("%s\n", name);
}

void display_none(char *name)
{
     int i,len;
    if(name[0] == '.')
        return;

    if(char_left < filename_max){
        printf("\n");
        char_left = MAXROWLEN;
    }
    
    len = strlen(name);
    len = filename_max - len;
    printf("%-s",name);
    
    //打印空格
    for(i = 0; i < len; i++)
        printf(" ");
    printf("  ");

    //获得剩余可用字符数
    char_left -= (filename_max+2);
}

void display_file(int param, char *path)
{
    struct stat buf;
    if(lstat(path,&buf) == -1){
        my_err("lstat",__LINE__);
    }
    
    if(param == 2){
        display_attribute(buf,path);
    }else if(param == 0){
        display_none(path);
    }else if(param == 1){
        display_single(path);
    }else {
        display_complex(buf,path);        
    }
}

//解析
void diaplay(int param,char *path)
{
    struct stat buf;
    char true_path[MAX+1];

    if(lstat(path,&buf) == -1){
        my_err("lstat",__LINE__);
    }
    if(S_ISDIR(buf.st_mode)){
        strcpy(true_path,path);
        if(true_path[0] != '/'){
            strcpy(true_path,getcwd(NULL,0));
            strcat(true_path,"/");
            strcat(true_path,path);
        }
        display_dir(param,true_path);
    }else{
        display_file(param,path);
    }
}

//解析目录
void display_dir(int param,const char *path)
{
    DIR *dir;
    struct dirent *pdir;
    struct stat buf;
    char filenames[FILE_MAX][MAX+1] = {0};
    char now_path[MAX+1],child_path[MAX+1];
    int i,count = 0,j;

    strcpy(now_path,getcwd(NULL,0));
    //修改工作目录到参数目录
    if(chdir(path) == -1){
        printf("这个:%s\n",path);
        my_err("chdir",__LINE__);
    }

    //打开参数目录
    if((dir = opendir(path)) == NULL){
        my_err("opendir",__LINE__);
    }

    //存储文件名及获得总文件数
    while((pdir = readdir(dir)) != NULL){
        if(lstat(pdir->d_name,&buf) == -1){
            my_err("readdir",__LINE__);
        }
        if(count >= FILE_MAX){
            printf(" %s目录下文件太多，无法全部显示!\n",path);
            break;
        }

        //获得最长的文件名
        if(strlen(pdir->d_name) > filename_max)
            filename_max = strlen(pdir->d_name);

        //复制文件名
        strcpy(filenames[count++],pdir->d_name);
    }

    //首先获得显示文件的顺序即排序
    if(param & PARAM_t){
        qsort(filenames,count,sizeof((char *)MAX),time_cmp);
    }else{
        qsort(filenames,count,sizeof(filenames[0]),str_cmp);
    }
    
    //在判断是否迭代显示即-R功能
 /*   if(param & PARAM_R){
        //先把当前目录下的所有文件或目录当做文件显示出来
        printf("%s:\n",path);
        for(i = 0; i < count; i++){
            display_none(filenames[i]);
        }
        printf("\n");

        //判断是否为目录
        for(i = 0; i < count; i++){
            if(filenames[i][0] == '.'){
                    continue;
            }
            
            if(!(S_ISDIR(buf.st_mode))){
                continue;
            }
            strcpy(child_path,path);
            if(path[strlen(path)-1] != '/'){
               strcat(child_path,"/"); 
            }
            strcat(child_path,filenames[i]);
            printf("\n\n");
            display_dir(param,child_path);
        }
    }else{*/
        for(j = 0; j < count; j++){
           display_file(param,filenames[j]);
        }
        printf("\n");

    //更改工作目录到父目录
    if(chdir(now_path) == -1){
        my_err("chdir",__LINE__);
    }
    closedir(dir);
}
