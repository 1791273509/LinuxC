/*************************************************************************
	> File Name: thread_once_run.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年08月01日 星期一 10时21分33秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void run(void);
void *thread1(void *arg);
void *thread2(void *arg);

void *thread2(void *arg)
{
    pthread_t thid = pthread_self();

    printf("current thread's ID is %u\n", thid);
    pthread_once(&once, run);
    printf("thread2 ends\n");
}

void *thread1(void *arg)
{
    pthread_t thid = pthread_self();

    printf("current thread's ID is %u\n", thid);
    pthread_once(&once, run);
    printf("thread1 ends\n");
}

void run(void)
{
    printf("function run is running in thread: %u\n", pthread_self());
}

int main(int argc,char *argv[])
{
    pthread_t thid1;
    pthread_t thid2;

    pthread_create(&thid1, NULL, thread1, NULL);
    pthread_create(&thid2, NULL, thread2, NULL);

    sleep(3);
    printf("main thread exit!\n");
    exit(0);
}
