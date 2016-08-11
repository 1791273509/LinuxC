/*************************************************************************
	> File Name: my_recv.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: Tue 09 Aug 2016 07:31:58 PM CST
 ************************************************************************/
#define MY_RECV_C

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include"my_recv.h"


//函数名: my_recv
//描述: 这是一个封装了函数recv的自定义读取数据的函数, 实际上是将套接字缓冲区中的数据拷贝到自定义缓冲区, 
//      然后再按格式(以'\n'为结束标志)读取出数据
//参数: conn_fd: 从该连接套接字上接收数据
//      data_buf: 读取到的数据保存在此缓冲区中
//      len: data_buf缓冲区的长度
//返回值: 出错返回-1, 服务器端关闭返回0, 成功返回读到的字节数

void my_error(const char *err_string, int line);
int my_recv(int conn_fd, char *data_buf, int len);

void my_error(const char *err_string, int line)
{
    fprintf(stderr, "line:%d ", line);
    perror(err_string);
    exit(1);
}

int my_recv(int conn_fd, char *data_buf, int len)
{
    static char recv_buf[BUFSIZE];      //自定义缓冲区, BUFSIZE定义在my_recv.h里面
    static char *pread;                 //指向下一次读取数据的位置
    static int len_remain = 0;          //自定义缓冲区剩余字节数
    int i;

    //如果自定义缓冲区中没有数据, 则从套接字读取数据
    if(len_remain < 0 || len_remain == 0)
    {
        if((len_remain = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
        {
            my_error("recv", __LINE__);
        }
        else if(len_remain == 0)    //recv函数的返回值, 若成功将套接字上的数据读到缓冲区内, 则返回读到的字节数,
        {                           //若服务器端已关闭连接, 则返回0,
            return 0;               //所以当len_remain为0时, 说明服务器端的socket连接关闭
        }

        pread = recv_buf;           //重新初始化pread指针, pread指向下一次读取数据的位置
    }

    //从自定义缓冲区中读取一次数据
    for(i = 0; *pread != '\n'; i++)
    {
        if(i > len)     //防止指针越界
        {
            return -1;
        }
        data_buf[i] = *pread++;
        len_remain--;
    }
    
    //去除结束标志
    len_remain--;
    pread++;

    return i;   //读取成功, i为读取到的字节数
}
