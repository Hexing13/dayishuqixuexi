/*************************************************************************
	> File Name: memory.c
	> Author: 
	> Mail: 
	> Created Time: 2015年12月02日 星期三 20时26分09秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<ctype.h>
#include<pthread.h>
#include<fcntl.h>
#include<semaphore.h>

#define PROCESS_NAME_LEN 32   /*进程名长度*/
#define MIN_SLICE    10             /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE 1024     /*内存大小*/
#define DEFAULT_MEM_START 0       /*起始位置*/

/* 内存分配算法 */
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3

int mem_size=DEFAULT_MEM_SIZE; /*内存大小*/
int ma_algorithm = MA_FF;      /*当前分配算法*/
static int pid = 0;            /*初始pid*/
int flag = 0;                 /*设置内存大小的标志*/

/*描述每一个空闲块的数据结构*/
struct free_block_type{
    int size;
    int start_addr;
    struct free_block_type *next;
};  

/*指向内存中空闲块链表的首指针*/
struct free_block_type *free_block = NULL;

/*每个进程分配到的内存块的描述*/
struct allocated_block{
    int pid;    
    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block *next;
};

/*进程分配内存块链表的首指针*/
struct allocated_block *allocated_block_head = NULL;

/*初始化空闲块，默认为一块*/
struct free_block_type *init_free_block(int mem_size){
    struct free_block_type *fb;
    fb=(struct free_block_type *)malloc(sizeof(struct free_block_type));
    if(fb==NULL){
        printf("No mem\n");
        return NULL;
    }
    fb->size = mem_size;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    return fb;
}

/*显示菜单*/
display_menu(){
    printf("\n");
    printf("1 - Set memory size (default=%d)\n", DEFAULT_MEM_SIZE);
    printf("2 - Select memory allocation algorithm\n");
    printf("3 - New process \n");
    printf("4 - Terminate a process \n");
    printf("5 - Display memory usage \n");
    printf("0 - Exit\n");
}

/*设置内存的大小*/
set_mem_size(){
    int size;

    if(flag!=0){  //防止重复设置
        printf("Cannot set memory size again\n");
        return 0;
    }
    printf("Total memory size = ");
    scanf("%d", &size);
    if(size>0) {
        mem_size = size;
        free_block->size = mem_size;
    }
    flag=1;  
    return 1;
}

/* 设置当前的分配算法 */
set_algorithm(){
    int algorithm;
    printf("\t1 - First Fit\n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit \n");
    scanf("%d", &algorithm);
    if(algorithm>=1 && algorithm <=3)  
        ma_algorithm=algorithm;
	//按指定算法重新排列空闲区链表
    rearrange(ma_algorithm); 
}

/*按指定的算法整理内存空闲块链表*/
rearrange(int algorithm){
    switch(algorithm){
        case MA_FF:  rearrange_FF(); break;
        case MA_BF:  rearrange_BF(); break;
        case MA_WF: rearrange_WF(); break;
    }
}

//按FF算法重新整理内存空闲块链表
rearrange_FF(){
    struct free_block_type *p0,*p1;
    int size;
    int start_addr;
    for(p0=free_block; p0!=NULL;p0=p0->next)
        for(p1 = p0->next; p1!=NULL;p1=p1->next){
            if(p0->start_addr > p1->start_addr){//交换 
                size = p1->size;
                start_addr = p1->start_addr;
                p1->size = p0->size;
                p1->start_addr = p0->start_addr;
                p0->size = size;
                p0->start_addr = start_addr;
            }
        }
}

//按BF算法重新整理内存空闲块链表
rearrange_BF(){
    struct free_block_type *p0,*p1;
    int size;
    int start_addr;
    for(p0=free_block; p0!=NULL;p0=p0->next)
        for(p1 = p0->next; p1!=NULL;p1=p1->next){
            if(p0->size > p1->size){//交换 
                size = p1->size;
                start_addr = p1->start_addr;
                p1->size = p0->size;
                p1->start_addr = p0->start_addr;
                p0->size = size;
                p0->start_addr = start_addr;
            }
        }
}

//按WF算法重新整理内存空闲块链表
rearrange_WF(){
  struct free_block_type *p0,*p1;
    int size;
    int start_addr;
    for(p0=free_block; p0!=NULL;p0=p0->next)
        for(p1 = p0->next; p1!=NULL;p1=p1->next){
            if(p0->size < p1->size){//交换 
                size = p1->size;
                start_addr = p1->start_addr;
                p1->size = p0->size;
                p1->start_addr = p0->start_addr;
                p0->size = size;
                p0->start_addr = start_addr;
            }
        }
}

/*创建新的进程，主要是获取内存的申请数量*/
new_process(){
    struct allocated_block *ab;
    int size;    
    int ret;
    ab=(struct allocated_block *)malloc(sizeof(struct allocated_block));
    if(!ab) 
        exit(-5);
    ab->next = NULL;
    pid++;
    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid;    
    printf("Memory for %s:", ab->process_name);
    scanf("%d", &size);
    if(size>0) ab->size=size;
    ret = allocate_mem(ab);  /* 从空闲区分配内存，ret==1表示分配ok*/
	/*如果此时allocated_block_head尚未赋值，则赋值*/
    if((ret==1) &&(allocated_block_head == NULL)){ 
        allocated_block_head=ab;
        return 1;        
    }
    /*分配成功，将该已分配块的描述插入已分配链表*/
    else if (ret==1) {
        ab->next=allocated_block_head;
        allocated_block_head=ab;
        return 2;        }
    else if(ret==-1){ /*分配不成功*/
        printf("Allocation fail\n");
        free(ab);
        return -1;       
     }
    return 3;
}

/*分配内存模块*/
int allocate_mem(struct allocated_block *ab){
    struct free_block_type *fbt,*pre;
    int request_size = ab->size;
    int sizesum = 0;//计算现有内存之和
    int ret = -1;
    fbt = pre = free_block;//空闲链指针
    while(pre!=NULL){
        if(pre->size > request_size && (pre->size - request_size)>=MIN_SLICE){
            //1. 找到可满足空闲分区且分配后剩余空间足够大，则分割 
            ab->start_addr = pre->start_addr;//设置进程起始地址
            pre->size-=request_size;//改变分割大小
            pre->start_addr+=request_size;//改变分割后起始地址
            ret = 1;//分配成功标志
            break;
        }else if(pre->size >= request_size && (pre->size-request_size) < MIN_SLICE){
                //2.找到可满足空闲分区且但分配后剩余空间比较小，则一起分配
                ab->start_addr = pre->start_addr;
                ab->size = pre->size;//重新设置进程分配的内存大小
                fbt->next = pre->next;
                free(pre);
                ret = 1;
                break;
        }
        sizesum+=pre->size;
        fbt = pre;
        pre = pre->next;
    }

    //内存紧缩技术
    //思想:将现有进程的起始地址从０开始依次改变，最后将空闲连的起始地址改变，大小变为剩余内存大小之和
    if((-1)==ret && sizesum >= request_size){
        struct allocated_block *pal;
        struct free_block_type *p,*q;
        int addr = 0;//记录当前分配内存的起始地址
        pal = allocated_block_head;
        while(pal!=NULL){
            pal->start_addr = addr;
            addr = pal->start_addr+pal->size;
            pal = pal->next;
        }
        //释放当前空闲块
        q = free_block->next;
        while(q!=NULL){
            free_block->next = q->next;
            free(q);
            if(NULL==free_block->next) break;
            q = free_block->next;
        }
        
        free_block->size = sizesum;
        printf("free_block->size:%d\n",free_block->size);
        free_block->start_addr = addr;
        if(free_block->size-request_size >= MIN_SLICE){
            ab->start_addr = free_block->start_addr;//设置进程起始地址
            free_block->size -= request_size;//改变分割后大小
            free_block->start_addr += request_size;//改变分割后起始地址
            ret = 1;//分配成功标志
        }else{
            ab->start_addr = free_block->start_addr;
            ab->size = free_block->size;
            free_block->start_addr = 0;
            free_block->size = 0;
            ret = 1;
        }
    }

    // 4. 在成功分配内存后，应保持空闲分区按照相应算法有序    
    if(ma_algorithm == MA_FF)  rearrange_FF();
    else if(ma_algorithm == MA_BF) rearrange_BF();
    else rearrange_WF();
    return ret;
}

//查询pid对应的结点
struct allocated_block *find_process(int pid){
    struct allocated_block *p,*q = NULL;
    for(p = allocated_block_head;p!=NULL;p = p->next)
        if(pid == p->pid)
            q = p;
    return q;
}

//删除进程，归还分配的存储空间，并删除描述该进程内存分配的节点
kill_process(){
    struct allocated_block *ab;
    int pid;
    printf("Kill Process, pid=");
    scanf("%d", &pid);
    ab=find_process(pid);
    if(ab!=NULL){
        free_mem(ab); /*释放ab所表示的分配区*/
        dispose(ab);  /*释放ab数据结构节点*/
    }
}

//将ab所表示的已分配区归还，并进行可能的合并
int free_mem(struct allocated_block *ab){
    int algorithm = ma_algorithm;
    struct free_block_type *fbt, *pre, *work;
    fbt=(struct free_block_type*)malloc(sizeof(struct free_block_type));
    if(!fbt) return -1;
    fbt->size=ab->size;
    fbt->next = NULL;
    fbt->start_addr = ab->start_addr;//进行可能的合并，基本策略如下
    //1.将新释放的结点插入到空闲分区队列末尾 
    if(free_block == fbt)
    //经过调试发现如果内存分配完后进行内存回收时此时malloc分配的内存地址与free_block地址相同
        free_block = fbt;
    else{
        pre = free_block;
        while(pre!= NULL){
            work = pre;
            pre = pre->next;
        }
        work->next = fbt;
    }

    //2.对空闲链表按照地址有序排列，rearrange_FF按地址递增排序
    rearrange_FF();
    //3.检查并合并相邻的空闲分区   
    pre = free_block;
    work = pre->next;
    int size1;
    fbt = pre;
    while(work != NULL){
        int i = 0;
        size1 = pre->size+pre->start_addr;
        if(size1==work->start_addr){
            //如果第一个节点是回收节点    
            pre->size += work->size;
            pre->next = work->next;
            free(work);
            i = 1;//合并标志，如果等于１则下一次循环时pre不变，因为这个还可能和下一个相连
            if(pre->next == NULL)
                break;
        }
        if(i == 0){
            pre = pre->next;
            work = pre->next;
        }else{
            work = pre->next;
        }
    }
    //４．将空闲链表重新按照当前算法排序
    if(algorithm == MA_FF) rearrange_FF();
    else if(algorithm == MA_BF) rearrange_BF();
    else rearrange_WF();
}

/*释放ab数据结构节点*/
int dispose(struct allocated_block *free_ab){
    struct allocated_block *pre, *ab;
    if(free_ab == allocated_block_head) { /*如果要释放第一个节点*/
     allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
    }
    pre = allocated_block_head;  
    ab = allocated_block_head->next;
    while(ab!=free_ab){ pre = ab;  ab = ab->next; }
    pre->next = ab->next;
    free(ab);
    return 2;
}

/* 显示当前内存的使用情况，包括空闲区的情况和已经分配的情况 */
display_mem_usage(){
    struct free_block_type *fbt=free_block;
    struct allocated_block *ab=allocated_block_head;
    if(fbt==NULL) return(-1);
    printf("----------------------------------------------------------\n");

    //显示空闲区
    printf("Free Memory:\n");
    printf("%20s %20s\n", "      start_addr", "       size");
    while(fbt!=NULL){
        printf("%20d %20d\n", fbt->start_addr, fbt->size);
        fbt=fbt->next;
    }    

	//显示已分配区
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", " size");
    while(ab!=NULL){
        printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
        ab=ab->next;
    }
    printf("----------------------------------------------------------\n");
    return 0;
}
do_exit()
{
    struct free_block_type *p1,*p2;
    struct allocated_block *a1,*a2;
    p1 = free_block;
    if(p1!=NULL){
        p2 = p1->next;
        for(;p2!=NULL;p1=p2,p2=p2->next)
            free(p1);
        free(p1);
    }
    
    a1 = allocated_block_head;
    if(a1!=NULL){
        a2 = a1->next;
        for(;a2!=NULL;a1=a2,a2=a2->next)
            free(a1);
        free(a1);
    }
}

int main(){
    char choice;      
    pid = 0;
    free_block = init_free_block(mem_size); //初始化空闲区
    while(1) {
        display_menu();	//显示菜单
        choice=getchar();	//获取用户输入
        switch(choice){
            case '1': set_mem_size(); system("cls"); break; 	//设置内存大小
            case '2': set_algorithm();flag=1; system("cls"); break;//设置算法
            case '3': new_process(); flag=1; system("cls"); break;//创建新进程
            case '4': kill_process(); flag=1;  system("cls");  break;//删除进程
            case '5': display_mem_usage(); flag=1;break;	//显示内存使用
            case '0': do_exit(); exit(0);	//释放链表并退出
            default: break;      
        }    
        getchar();
    } 
}
