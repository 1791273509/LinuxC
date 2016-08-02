/*************************************************************************
	> File Name: condition.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年08月02日 星期二 14时42分24秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void *thread1(void *arg);
void *thread2(void *arg);

void *thread2(void *arg)
{
    while(1)
    {
        printf("thread2 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread2 applied the condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

void *thread1(void *arg)
{
    pthread_cleanup_push(pthread_mutex_unlock, &mutex);
    
    while(1)
    {
        printf("thread1 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread1 applied the condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(4);
    }

    pthread_cleanup_pop(0);
}

int main(int argc,char *argv[])
{
    pthread_t tid1;
    pthread_t tid2;
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    do
    {
        pthread_cond_signal(&cond);
    }while(1);

    sleep(50);
    pthread_exit(0);
}
