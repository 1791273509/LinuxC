/*************************************************************************
	> File Name: processimage.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月25日 星期一 22时23分45秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
    int i;

    printf("这是一个新的程序!\n");

    printf("新程序当前进程的ID:%d, 新程序的当前进程的父进程的ID:%d\n", getpid(), getppid());
    printf("新程序的uid:%d, 新程序的gid:%d\n", getuid(), getgid());
    
    for(i = 0; i < argc; i++)
    {
        printf("argv[%d]:%s\n", i, argv[i]);
    }
}
