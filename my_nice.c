/*************************************************************************
	> File Name: my_nice.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月26日 星期二 16时42分15秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/resource.h>
#include<sys/wait.h>
#include<errno.h>

int main(int argc,char *argv[])
{
    pid_t pid;
    int stat_val = 0;
    int oldpri;
    int newpri;

    pid = fork();
    switch(pid)
    {
        case 0 :
            printf("子进程正在运行,当前pid:%d, 父进程pid:%d\n", pid, getppid());
            
            oldpri = getpriority(PRIO_PROCESS, 0);
            printf("old priority = %d\n", oldpri);

            //newpri = nice(-6);
            newpri = nice(6);
            printf("new priority = %d\n", newpri);
        
            exit(0);

        case -1 :
            perror("进程创建失败");
            break;

        default :
            printf("父进程正在运行, 当前进程的pid:%d, 子进程的pid:%d\n", getpid(), pid);
            break;
    }

    wait(&stat_val);
    exit(0);

    //在上述代码中,newpri = nice(6); 输出的newpri是6, 而newpri = nice(-6); 则输出的newpri是-1,
    //这说明在执行这条语句时nice出错了
    //不能用errno检测-1, 因为 int prio 的取值时-20到20, 所以返回-1有可能是出错,也有可能是设置的优先级
    //又因为只要newpri = nice(负值); 输出的newpri都是-1,由此可判断是这条语句nice时出错
    //prio代表的是进程的执行优先权, 数值越低就代表有较高的优先次序, 执行会比较频繁,
    //此优先权默认是0, 而只有超级用户允许降低此值,这样就可以解释为什么newpri = nice(负值); 会一直出错了
}
