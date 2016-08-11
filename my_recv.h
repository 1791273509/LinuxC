/*************************************************************************
	> File Name: my_recv.h
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: Wed 10 Aug 2016 08:40:09 AM CST
 ************************************************************************/

#ifndef _MY_RECV_H
#define _MY_RECV_H
    #define BUFSIZE 1024
    void my_error(const char *err_string, int line);
    int my_recv(int conn_fd, char *data_buf, int len);
#endif
