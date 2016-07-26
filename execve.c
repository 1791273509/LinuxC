/*************************************************************************
	> File Name: execve.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月26日 星期二 08时12分26秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(int argc,char *argv[],char **environ)
{
    pid_t pid;
    int stat_val;
    printf("stat_val的初始值为:%d\n", stat_val);

    printf("execve调用的一个例子!\n");

    pid = fork();
    switch(pid)
    {
        case -1 :
            perror("进程创建失败!\n");
            exit(1);

        case 0 :
            printf("子进程开始运行!\n");
            printf("子进程的pid:%d, 父进程的pid:%d\n", getpid(), getppid());
            printf("子进程的uid:%d, 子进程的gid:%d\n", getuid(), getgid());

            execve("processimage", argv, environ);
            printf("永远都不会执行这条语句!\n");
            exit(0);

        default : 
            printf("父进程正在运行\n");
            printf("子进程的pid:%d, 父进程的pid:%d\n", pid, getpid());
            break;
    }

    wait(&stat_val);
    printf("当前stat_val的值为:%d\n", stat_val);
    exit(0);
}
