/*************************************************************************
	> File Name: join_thread.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年08月01日 星期一 20时10分06秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void assisthread(void *arg);

void assisthread(void *arg)
{
    printf("I am helping to do some jobs\n");
    sleep(3);
    pthread_exit(0);
}

int main(int argc,char *argv[])
{
    pthread_t assistthid;
    int status;

    pthread_create(&assistthid, NULL, (void *) assisthread, NULL);
    //pthread_join(assistthid, (void *) &status);

    printf("assisthread's exit is caused %d\n", status);

    return 0;
}
