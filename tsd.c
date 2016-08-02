/*************************************************************************
	> File Name: tsd.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年08月02日 星期二 08时41分48秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

pthread_key_t key;

void *thread1(void *arg);
void *thread2(void *arg);

void *thread2(void *arg)
{
    int tsd = 5;
    printf("thread2 %d is running\n", pthread_self());
    pthread_setspecific(key, (void *)tsd);
    printf("thread2 %d returns %d\n", pthread_self(), pthread_getspecific(key));
}

void *thread1(void *arg)
{
    int tsd = 0;
    pthread_t thid2;

    printf("thread1 %d is running\n", pthread_self());
    pthread_setspecific(key, (void *)tsd);
    pthread_create(&thid2, NULL, thread2, NULL);
    sleep(5);
    printf("thread1 %d returns %d\n", pthread_self(), pthread_getspecific(key));
}

int main(int argc,char *argv[])
{
    pthread_t thid1;
    printf("main thread begins running\n");
    
    pthread_key_create(&key, NULL);
    pthread_create(&thid1, NULL, thread1, NULL);
    
    sleep(10);
    pthread_join(thid1, NULL);
    pthread_key_delete(key);
    printf("mian thread exit\n");

    return 0;

}
