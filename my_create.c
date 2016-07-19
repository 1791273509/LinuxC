/*************************************************************************
	> File Name: my_create.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月19日 星期二 08时32分16秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

int main(int argc,char *argv[])
{
    int fd;

    if((fd = open("example6.2.c", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) == -1)
    //if((fd = creat("example6.2.c", S_IRWXU)) == -1)
    {
        perror("open");
        //printf("open:%s   with errno:%d\n", strerror(errno), errno);

        exit(1);
    }
    else
    {
        printf("create file success\n");
    }

    close(fd);
    return 0;
}
