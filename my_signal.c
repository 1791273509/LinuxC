/*************************************************************************
	> File Name: my_signal.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: Mon 26 Sep 2016 09:03:57 PM CST
 ************************************************************************/

#include<stdio.h>
#include<signal.h>

void handler_sigint(int signo);

void handler_sigint(int signo)
{
    printf("recv SIGINT\n");
}

int main(int argc,char *argv[])
{
    signal(SIGINT, handler_sigint);
    
    //SIGINT: 即ctrl+c, 用户终端向正在运行中的由改宏端启动的程序发送此信号, 默认动作为终止进程 
    
    while(1)
    {
        ;
    }

    return 0;
}
