/*************************************************************************
	> File Name: chatServer.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: Wed 17 Aug 2016 09:03:30 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<pthread.h>
#include<fcntl.h>
#include<dirent.h>

#define BUFSIZE     1024

#define SERV_PORT   4507
#define LISTENQ     300 

#define LOG         '1'
#define REG         '2'

#define FAIL        'n'
#define SUCCESS     'y'

typedef struct DATE         //日期结构体
{
    int month;
    int day;
}DATE;

typedef struct FRIENDINFO   //好友列表结构体
{
    int usernum;
    char name[20];
}FRIENDINFO;

typedef struct USERINFO     //用户基本信息结构体
{
    int usernum;
    char username[20];      //注册时, 昵称不能超过八个字
    char sex[4];            //年龄
    int age;                //性别
    DATE date;               //生日
    char constell[10];       //星座
    char sentence[40];      //个性签名, 不超过十五个字, 设置个性签名时应该检查字数
}USERINFO;

typedef struct REGINFO      //用户注册信息结构体
{
    char password[17];          //不超过16位的密码
    USERINFO userinfo;          //用户基本信息
    FRIENDINFO *friendlist;     //好友列表
}REGINFO;

typedef struct LOGINFO      //用户登录结构体
{
    int usernum;
    char password[17];
    char state;             //状态
}LOGINFO;
typedef struct MESSAGE
{
    FRIENDINFO friendinfo;  //好友的信息,包含好友的帐号,好友的名字,根据好友帐号查询他的conn_fd,名字提供聊天框信息
    char messagecontent[BUFSIZE];   //消息内容  
}MESSAGE;

typedef struct ALL
{
    char mark;              //标志数据类型
    REGINFO reginfo;        //注册结构体
    LOGINFO loginfo;        //登录结构体
    USERINFO userinfo;      //存储用户个人信息的结构体
    USERINFO friendallinfo; //存储好友的个人完整信息的结构体
    MESSAGE message;
}ALL;

typedef struct ONLINELIST
{
    int conn_fd;
    int usernum;
    char state;
    struct ONLINELIST *next;
}ONLINELIST;

int conn_fd;
int number = 16081101;
char getResult;
ONLINELIST *q;

void my_error(const char *err_string, int line);
void server_init(int argc, char *argv[]);
void server_main(ONLINELIST *head);
void createDirAndFile(ALL regAll);
void itoa(unsigned long val, char *buf, unsigned radix);
int getFileLen(int fd);
void addOnlineList(ONLINELIST *head, ALL logAll, int conn_fd);

void addOnlineList(ONLINELIST *head, ALL logAll, int conn_fd)
{
    ONLINELIST *p;

    p = (ONLINELIST *)malloc(sizeof(ONLINELIST));
    
    p->conn_fd = conn_fd;
    p->usernum = logAll.loginfo.usernum;
    p->state = logAll.loginfo.state;
    p->next = NULL;

    if(head->next == NULL)
    {
        head->next = p;
    }
    else
    {
        q->next = p;
    }
    q = p;
}

int getFileLen(int fd)
{
    int len;

    if(lseek(fd, 0, SEEK_END) == -1)
    {
        my_error("lseek", __LINE__);
    }
    if((len = lseek(fd, 0, SEEK_CUR)) == -1)
    {
        my_error("lseek", __LINE__);
    }
    if((lseek(fd, 0, SEEK_SET)) == -1)
    {
        my_error("lseek", __LINE__);
    }
    
    return len;
}

void itoa(unsigned long val, char *buf, unsigned radix) 
{ 
    char *p;  
    char *firstdig; 
    char temp; 
    unsigned digval; 

    p = buf; 
    firstdig = p; 

    do { 
        digval = (unsigned) (val % radix); 
        val /= radix; 

        if (digval > 9) 
        *p++ = (char ) (digval - 10 + 'a'); 
        else 
        *p++ = (char ) (digval + '0');
    } while (val > 0); 


    *p-- = '\0'; 

    do { 
        temp = *p; 
        *p = *firstdig; 
        *firstdig = temp; 
        --p; 
        ++firstdig; 
    } while (firstdig < p); 
}

void createDirAndFile(ALL regAll)
{
    USERINFO userInfo;

    char userDirName[255];
    char tmpName[30] = {"chat/"};
    char passwordBuf[20] = {0};
    
    int userInfoFd;
    int passwordFd;
    int friendlistFd;
    int chatRecordFd;
    
    char fileUserInfo[4096] = {0};
    char filePassword[4096] = {0};
    char fileFriendlist[4096] = {0};
    char dirChatRecord[4096] = {0};


    //先要创建用户主目录;
    itoa(number, userDirName, 10);
    printf("userDirName:%s\n", userDirName);
    strcat(tmpName, userDirName);
    printf("用户主目录:%s\n", tmpName);

    mkdir(tmpName, 0777);

    //然后再创建用户目录下的一系列文件
    strcpy(fileUserInfo, tmpName);
    strcpy(filePassword, tmpName);
    strcpy(fileFriendlist, tmpName);
    strcpy(dirChatRecord, tmpName);

    strcat(fileUserInfo, "/userinfo.txt");
    strcat(filePassword, "/password.txt");
    strcat(fileFriendlist, "/friendlist.txt");
    strcat(dirChatRecord, "/chatrecord");

    //printf("%s\n", fileUserInfo);
    //printf("%s\n", filePassword);
    //printf("%s\n", fileFriendlist);
    //printf("%s\n", dirChatRecord);

    //创建用户信息文件
    if((userInfoFd = open(fileUserInfo, O_RDWR | O_CREAT, 0777)) < 0)
    {
        my_error("open", __LINE__);
    }

    userInfo = regAll.reginfo.userinfo;

    if(write(userInfoFd, &userInfo, sizeof(USERINFO)) != sizeof(USERINFO))
    {
        my_error("write", __LINE__);
    }
    
    close(userInfoFd);
    
    //创建用户密码文件
    if((passwordFd = open(filePassword, O_RDWR | O_CREAT, 0777)) < 0)
    {
        my_error("write", __LINE__);
    }

    strcpy(passwordBuf, regAll.reginfo.password);

    if(write(passwordFd, passwordBuf, strlen(passwordBuf)) != strlen(passwordBuf))
    {
        my_error("write", __LINE__);
    }

    close(passwordFd);
    
    //创建好友列表文件
    if((friendlistFd = open(fileFriendlist, O_RDWR | O_CREAT, 0777)) < 0)
    {
        my_error("open", __LINE__);
    }

    close(friendlistFd);

    //创建聊天记录总目录
    mkdir(dirChatRecord, 0777);
}

void server_main(ONLINELIST *head)
{
    char oneMark;       //用于登录注册的第一层标志
    char endMark;       //用于判断客户端是否挂掉的标志
    ALL logAll;         //接收登录信息的结构体
    ALL regAll;         //接收注册信息的结构体
    ALL userinfoAll;    //发送给客户端的用户信息
    char fileUserMainName[255] = {"chat/"};     //存放当前登录的用户的主目录的名字
    char filePasswordName[255];                 //密码文件的文件名
    char fileUserinfoName[255];                 //用户基本信息的文件名
    char tmpUsernumChar[20];    //暂时存放当前登录的用户帐号的字符形式, 以便打开该用户的主目录
    char sysNumberChar[20];     //存放系统分配的帐号的字符形式
    DIR *dir;
    struct dirent *dirent;
    int passwordFd;         //密码文件的文件描述符
    int fileLen;            //计算各种文件内容的长度
    int ret;                //读文件时读到的字节数
    char passwordBuf[30];   //将读取密码文件时读出的密码暂时存放在这个缓冲区中
    int sysNumberFd;
    int userinfoFd;

    //先与客户端进行交互, 完成登录注册
    while(1)
    {
        endMark = -1;
        oneMark = 'z';

        //分登录和注册两种情况处理一下
        if((endMark = recv(conn_fd, &oneMark, sizeof(oneMark), 0)) < 0)
        {
            my_error("recv", __LINE__);
        }
        else if(endMark == 0)
        {
            pthread_exit(0);
        }
        else
        {
            if(oneMark == LOG)
            {
                //登录
                if(recv(conn_fd, &logAll, sizeof(ALL), 0) < 0)
                {
                    my_error("recv", __LINE__);
                }

                //在chat目录下找到该帐号的主目录, 然后读取出密码文件
                itoa(logAll.loginfo.usernum, tmpUsernumChar, 10);
                strcat(fileUserMainName, tmpUsernumChar);
                printf("###这里是服务器登录功能,当前得到的用户的主目录名字为:%s\n", fileUserMainName);

                //打开该用户的主目录, 找到密码文件, 读取出密码, 与客户端发送过来的信息进行匹配
                if((dir = opendir(fileUserMainName)) == NULL)
                {
                    //my_error("opendir", __LINE__);
                    getResult = FAIL;
                }
                else
                {
                    while((dirent = readdir(dir)) != NULL)
                    {
                        if(!strcmp(dirent->d_name, "password.txt"))
                        {
                            printf("密码文件的名字:%s\n", dirent->d_name);
                            strcpy(filePasswordName, fileUserMainName);
                            strcat(filePasswordName, "/password.txt");

                            if((passwordFd = open(filePasswordName, O_RDWR)) < 0)
                            {
                                my_error("open", __LINE__);
                                fileLen = getFileLen(passwordFd);

                                if((ret = read(passwordFd, passwordBuf, fileLen)) < 0)
                                {
                                    my_error("read", __LINE__);
                                }
                                printf("###当前登录的用户的密码为:%s\n", passwordBuf);

                                if(!strcmp(passwordBuf, logAll.loginfo.password))
                                {
                                    getResult = SUCCESS;
                                }
                                break;
                            }
                        }
                    }

                    //读取结果用getResult发送
                    //若帐号不存在或帐号存在密码不正确则返回n
                    if(send(conn_fd, &getResult, sizeof(getResult), 0) < 0)
                    {
                        my_error("send", __LINE__);
                    }
                    
                    //如果登录成功了, 那么就将当前登录的用户的信息发送过去, 
                    //并且将该客户端的信息存储再当前在线信息链表中
                    if(getResult == SUCCESS)
                    {
                        strcpy(fileUserinfoName, fileUserMainName);
                        strcat(fileUserinfoName, "/userinfo.txt");
                        
                        if((userinfoFd = open(fileUserinfoName, O_RDWR)) < 0)
                        {
                            my_error("open", __LINE__);
                        }
                        printf("用户信息的文件已经打开\n");
                        if((ret = read(userinfoFd, &userinfoAll.userinfo, sizeof(USERINFO))) < 0)
                        {
                            my_error("read", __LINE__);
                        }
                        printf("用户信息的文件内容已经读出\n");
                        close(userinfoFd);
                        printf("用户信息的文件已经关闭");

                        if(send(conn_fd, &userinfoAll, sizeof(ALL), 0) < 0)
                        {
                            my_error("send", __LINE__);
                        }
                        printf("用户信息已经发送给客户端\n");
                        
                        //将该客户端的信息存储在当前在线信息链表中
                        addOnlineList(head, logAll, conn_fd);
                    }
                }
                closedir(dir);
            }
            else if(oneMark == REG)
            {
                //注册 
                
                if((sysNumberFd = open("chat/number.txt", O_RDWR)) < 0)
                {
                    my_error("open", __LINE__);
                }

                fileLen = getFileLen(sysNumberFd);

                if((ret = read(sysNumberFd, sysNumberChar, fileLen)) < 0)
                {
                    my_error("read", __LINE__);
                }
                printf("当前要分配的帐号为:%s\n", sysNumberChar);
                number = atoi(sysNumberChar);
                printf("将当前要分配的帐号转换为数字形式\n");

                if(send(conn_fd, &number, sizeof(number), 0) < 0)
                {
                    my_error("send", __LINE__);
                }

                close(sysNumberFd);

                //这时客户端将注册的信息已经全部填好并要发送过来
                //所以服务器接收注册的信息
                if(recv(conn_fd, &regAll, sizeof(ALL), 0) < 0)
                {
                    my_error("recv", __LINE__);
                }
                printf("接收注册信息成功!\n");

                printf("number :%d\n", regAll.reginfo.userinfo.usernum);
                printf("username :%s\n", regAll.reginfo.userinfo.username);
                printf("password :%s\n", regAll.reginfo.password);
                printf("age :%d\n", regAll.reginfo.userinfo.age);
                printf("sentence :%s\n", regAll.reginfo.userinfo.sentence);
                printf("sex :%s\n", regAll.reginfo.userinfo.sex);
                printf("month :%d\n", regAll.reginfo.userinfo.date.month);
                printf("day :%d\n", regAll.reginfo.userinfo.date.day);
                printf("constell :%s\n",regAll.reginfo.userinfo.constell);

                //对这个结构体进行一系列处理
                //然后建目录文件等

                printf("创建文件之前\n");
                createDirAndFile(regAll);

                //建好目录文件后, 给客户端返回一个成功的标志
                getResult = SUCCESS;
                
                if(send(conn_fd, &getResult, sizeof(getResult), 0) < 0)
                {
                    my_error("send", __LINE__);
                }

                //再次把存有系统分配帐号的文件以清空方式打开, 再将下一次要分配的帐号写进去
                number++;

                if((sysNumberFd = open("chat/number.txt", O_RDWR | O_TRUNC)) < 0)
                {
                    my_error("open", __LINE__);
                }
                itoa(number, sysNumberChar, 10);
                printf("下次注册时的帐号是:%s\n", sysNumberChar);
                if(write(sysNumberFd, sysNumberChar, strlen(sysNumberChar)) != strlen(sysNumberChar))
                {
                    my_error("write", __LINE__);
                }
                close(sysNumberFd);
            }
        }
    }

    //然后再创建一个recv线程
    //再写一个dealMain处理mark的全部情况
    }

void server_init(int argc, char *argv[])
{
    int sock_fd;                    //套接字
    int optval;                     //启用套接字选项
    struct sockaddr_in cli_addr;    //客户顿地址结构
    struct sockaddr_in serv_addr;   //服务器端地址结构
    socklen_t cli_len;              //客户端地址结构长度
    char recv_buf[128];             //接收数据的缓冲区
    int ret;
    pthread_t pid;                  //线程id
    int fd;                         //帐号文件的文件描述符    
    char buf[64];
    ONLINELIST head = {0};          //创建存储当前链接到服务器的客户端信息的链表

    //创建一个总目录
    mkdir("chat", 0777);

    //在chat目录下建立一个存储分配帐号信息的文件
    
    if((fd = open("chat/number.txt", O_RDWR)) < 0)
    {
        if((fd = open("chat/number.txt", O_RDWR|O_CREAT, 0777)) < 0)
        {
            my_error("open", __LINE__);
        }

        itoa(number, buf, 10);

        if(write(fd, buf, strlen(buf)) != strlen(buf))
        {
            my_error("write", __LINE__);
        }

        close(fd);
    }
    close(fd);

    //创建一个套接字
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
        my_error("socket", __LINE__);
    }

    //设置套接字, 使之可以重新绑定端口
    optval = 1;         //将该值置为非0值, 启用套接字选项
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
    {
        my_error("setsockopt", __LINE__);
    }

    //初始化服务器结构
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //将套接字绑定到本地端口上
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
        //通过accept接收客户端的连接请求, 并返回套接字用于收发数据
        printf("连接之前!\n");
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
        printf("有没有连接成功!\n");
        if(conn_fd < 0)
        {
            my_error("accept", __LINE__);
        }

        printf("接收到了一个新的客户端, ip地址是:%s\n", inet_ntoa(cli_addr.sin_addr));
        
        pthread_create(&pid, NULL, (void *)server_main, &head);
        printf("线程创建之后!\n");
    }
}

void my_error(const char *err_string, int line)
{
    fprintf(stderr, "line:%d ", line);
    perror(err_string);
    exit(1);
}
 
int main(int argc,char *argv[])
{
    server_init(argc, argv);
}
