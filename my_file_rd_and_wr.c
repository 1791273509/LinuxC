/*************************************************************************
	> File Name: my_file_rd_and_wr.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月19日 星期二 10时20分19秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

void my_error(const char *err_string, int line);
int my_read(int fd);

//自定义的读数据的函数
int my_read(int fd)
{
    int len;              //文件的行数
    int ret;              //文件的字节数
    int i;
    char read_buf[64];    //模拟键盘缓冲区

    //获取文件长度并保持文件读写指针在文件开始处
    if(lseek(fd, 0, SEEK_END) == -1)
    {
        my_error("lseek",__LINE__);
    }
    if((len = lseek(fd, 0, SEEK_CUR)) == -1)
    {
        my_error("lseek", __LINE__);
    }
    if(lseek(fd, 0, SEEK_SET) == -1)
    {
        my_error("lseek", __LINE__);
    }

    printf("len:%d\n", len);

    //读数据
    if((ret = read(fd, read_buf, len)) < 0)
    {
        my_error("read", __LINE__);
    }

    //打印数据
    for(i = 0; i < len; i++)
    {
        printf("%c", read_buf[i]);
    }
    printf("\n");

    return ret;
}

//自定义的错误处理函数
void my_error(const char *err_string , int line)
{
    fprintf(stderr, "line:%d  ", line);
    perror(err_string);
    exit(1);
}

int main(int argc,char *argv[])
{
    int fd;
    char write_buf[32] = "hello world!";

    //在当前的目录下创建example6.3.c
    //if((fd = open("example6.3.c", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU)) == -1)
    if(fd = creat("example6.3.c", S_IRWXU) == -1)
    {
        my_error("open", __LINE__);
    }
    else
    {
        printf("file create success\n");
    }

    //写数据
    if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf))
    {
        my_error("write", __LINE__);
    }
    my_read(fd);

    //演示文件的间隔
    printf("/*----------------*/\n");
    if(lseek(fd, 10 ,SEEK_END) == -1)
    {
        my_error("lseek", __LINE__);
    }

    if(write(fd, write_buf, strlen(write_buf)) == -1)
    {
        my_error("write", __LINE__);
    }

    my_read(fd);

    close(fd);

    return 0;
}
