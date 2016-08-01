/*************************************************************************
	> File Name: create_thread.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年08月01日 星期一 09时52分15秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int *thread(void *arg);

int *thread(void *arg)
{
    pthread_t newthid;

    newthid = pthread_self();
    printf("this is a new thread, thread ID = %u\n", newthid);

    return NULL;
}

int main(void)
{
    pthread_t thid;

    printf("main thread, ID is %u\n", pthread_self());

    if(pthread_create(&thid, NULL, (void *) thread, NULL) != 0)
    {
        printf("tread creation failed\n");
        exit(1);
    }

    sleep(1);
    exit(0);
}
