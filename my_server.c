/*************************************************************************
	> File Name: my_server.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: Tue 09 Aug 2016 04:00:00 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include"my_recv.h"                 //自定义的头文件

#define SERV_PORT  4507          //服务器的端口
#define LISTENQ       12            //连接请求队列的最大长度

#define INVALID_USERINFO  'n'       //用户信息无效
#define VALID_USERINFO    'y'       //用户信息有效

#define USERNAME  0                 //接收到的是用户名
#define PASSWORD  1                 //接收到的是密码

typedef struct userinfo                     //保存用户名和密码的结构体
{
    char username[32];
    char password[32];
}userinfo;

userinfo users[] = 
{
    {"linux", "unix"},
    {"4507", "4508"},
    {"clh", "clh"},
    {"xl", "xl"},
    {" ", " "}                      //以只含一个空格符的字符串作为数组的结束标识
};

int find_name(const char *name);
void send_data(int conn_fd, const char *string);

void send_data(int conn_fd, const char *string)
{
    if(send(conn_fd, string, strlen(string), 0) < 0)
    {
        my_error("send", __LINE__);
    }
}

int find_name(const char *name)
{
    int i;

    if(name == NULL)
    {
        printf("in find_name, NULL pointer\n");
        return -2;
    }

    for(i = 0; users[i].username[0] != ' '; i++)
    {
        if(!strcmp(users[i].username, name))
        {
            return i;
        }
    }

    return -1;
}

int main(int argc,char *argv[])
{
    int sock_fd;                     //创建一个套接字的描述符
    int conn_fd;                    //连接套接字, 用于收发数据
    int optval;                     //启用套接字选项的标识, 0为禁止套接字选项, 非0为启用套接字选项
    int flag_recv = USERNAME;       //标识接收到的是用户名还是密码
    int ret;                        //接收recv函数返回值, 即接收到的数据的字节数
    int name_num;                   //通过find_name找到的userinfo结构体的数组的下标
    pid_t pid;
    struct sockaddr_in cli_addr;    //客户端地址结构体
    struct sockaddr_in serv_addr;   //服务器地址结构体
    socklen_t cli_len;              //客户端地址结构体的长度
    char recv_buf[128];             //自定义缓冲区

    //创建一个套接字
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
        my_error("socket", __LINE__);
    }

    //设置该套接字使之可以重新绑定端口
    optval  =1;                     //将optval设置为非0值, 启用套接字选项

    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
    {
        my_error("setsockopt", __LINE__);
    }

    //初始化服务器端地址结构
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //将套接字绑定到本地端口
    if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
    {
        my_error("bind", __LINE__);
    }

    //将套接字转化为监听套接字
    if(listen(sock_fd, LISTENQ) < 0)
    {
        my_error("listen", __LINE__);
    }

    cli_len = sizeof(struct sockaddr_in);

    while(1)
    {
        //通过accept接收客户端的连接请求, 并返回连接套接字用于收发数据
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
        if(conn_fd < 0)
        {
            my_error("accept", __LINE__);
        }
        printf("accept a new client, ip is %s\n", inet_ntoa(cli_addr.sin_addr));

        //创建一个子进程处理刚刚接收的连接请求
        if((pid = fork()) == 0)         //子进程
        {
            while(1)
            {
                if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
                {
                    perror("recv");
                    exit(1);
                }
                recv_buf[ret-1] = '\0';     //将数据结束标志'\n'替换成字符串结束标志
                
                if(flag_recv == USERNAME)   //接收到的是用户名
                {
                    name_num = find_name(recv_buf);

                    switch(name_num)
                    {
                        case -1:
                            send_data(conn_fd, "n\n");
                            break;

                        case -2:
                            exit(1);
                            break;

                        default:
                            send_data(conn_fd, "y\n");
                            flag_recv = PASSWORD;
                            break;
                    }
                }
                else if(flag_recv == PASSWORD)  //接收到的是密码      
                {
                    if(!strcmp(users[name_num].password, recv_buf))
                    {
                        send_data(conn_fd, "y\n");
                        send_data(conn_fd, "welcome login my tcp server\n");
                        printf("%s login\n", users[name_num].username);
                        break;
                    }
                    else
                    {
                        send_data(conn_fd, "n\n");
                    }
                }  
            }
            close(sock_fd);
            close(conn_fd);
            exit(0);
        }
        else    //父进程关闭刚刚接收的连接请求, 执行accept等待其他连接请求
        {
            close(conn_fd);
        }
    }
    return 0;
}
