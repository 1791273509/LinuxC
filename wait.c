/*************************************************************************
	> File Name: wait.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月26日 星期二 11时11分35秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc,char *argv[])
{
    pid_t pid;
    char *msg;
    int k;
    int exit_code;   //退出码

    printf("如何得到退出码\n");
    pid = fork();

    switch(pid)
    {
        case 0 : 
            msg = "子进程正在运行\n";
            k = 5;
            exit_code = 37;
            break;

        case -1 :
            perror("进程创建失败!\n");
            exit(1);

        default :
            exit_code = 0;
            break;
    }

    //父子进程都会运行到下面这段代码,pid == 0时为子进程, pid != 0时为父进程
    if(pid != 0)        //父进程等待子进程结束
    {
        int stat_val;
        pid_t child_pid;

        child_pid = wait(&stat_val);

        printf("子进程已经结束, pid:%d\n", child_pid);
        
        if(WIFEXITED(stat_val))
        {
            printf("子进程的退出码:%d\n", WEXITSTATUS(stat_val));
        }
        else
        {
            printf("子进程是不正常退出的\n");
        }
    }
    else
    {
        while(k-- > 0)
        {
            puts(msg);
            sleep(1);
        }
    }        
    
    exit(exit_code);
}
