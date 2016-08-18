/*************************************************************************
	> File Name: chat_server.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: Fri 12 Aug 2016 09:54:19 AM CST
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

#define SERV_PORT   4507
#define LISTENQ     300 

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

typedef struct ONLINELIST
{
    int conn_fd;
    int usernum;
    char state;
    struct ONLINELIST *next;
}ONLINELIST;

int number = 16081101;
int conn_fd;
ONLINELIST *q;

void my_error(const char *err_string, int line);
void server_init(int argc, char *argv[]);
void *server_main(ONLINELIST *head);
void createDirAndFile(REGINFO regInfo);
void itoa(unsigned long val, char *buf, unsigned radix);
int getFileLen(int fd);

void addOnlineList(ONLINELIST *head, LOGINFO logInfo, int conn_fd);

void addOnlineList(ONLINELIST *head, LOGINFO logInfo, int conn_fd)
{
    ONLINELIST *p;

    p = (ONLINELIST *)malloc(sizeof(ONLINELIST));
    
    p->conn_fd = conn_fd;
    p->usernum = logInfo.usernum;
    p->state = logInfo.state;
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

void createDirAndFile(REGINFO regInfo)
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

    userInfo = regInfo.userinfo;

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

    strcpy(passwordBuf, regInfo.password);

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

void *server_main(ONLINELIST *head)
{
    int mark;           //判断客户端是否关掉的标志
    char oneMark;
    char getResult;
    USERINFO userInfo;
    REGINFO regInfo;
    LOGINFO logInfo;
    char numberBuf[64] = {0}; //将系统将要分配给客户端的帐号暂时读取到这个缓冲区中
    int fileLen;
    int ret;            //读到的字节数
    int sysNumberFd;
    DIR *dir;               //DIR是一个目录流
    struct dirent *dirent;  //读取目录时回返回一个存有文件信息的指针
    char dirUserMain[255] = {0};  //存储用户主目录名 
    char passwordBuf[20] = {0};   //将用户主目录下的密码文件暂时读取到这个缓冲区中
    char filePassword[255] = {0};
    int passwordFd;         //密码文件描述符
    char dirTmpName[255] = {"chat/"};
    char curCwd[255];           //获取当前目录
    char tmpCurCwd[255] = {0};
    char tmpUsernum[20];    //临时存放登录上来的用户的帐号, 以便查找有关此用户的信息
    int intTmpUserinfo;     //临时存放登录上来的帐号, 以int形式存放
    int userinfoFd;
    char tmpFileName[255];  //用来存放各种临时的文件名
    char twoMark;
    char threeMark;
    char fourMark;

    strcpy(curCwd, getcwd(NULL, 0));
    strcat(curCwd, "/");

    printf("cwd:%s\n", curCwd);
    while(1)
    {
        mark = -1;
        oneMark = 'z';      //oneMark的初始化

        //判断消息类型, 将消息分成类, 然后在server_main里面按类处理
        if((mark = recv(conn_fd, &oneMark, sizeof(oneMark), 0)) < 0)
        {
            my_error("recv", __LINE__);
        }
        else if(mark == 0)
        {
            pthread_exit(0);
        }
        else
        {
            printf("else大循环要开始了\n");
            printf("第一层标志位为:%c\n", oneMark);
            /*if(oneMark == '0')
            {
                //标志位为0, 执行下线
                pthread_exit(0);
            }*/
            if(oneMark == '1')
            {
                //标志位为1, 执行登录
                //需要接收一个LOGINFO的结构体
                if(recv(conn_fd, &logInfo, sizeof(LOGINFO), 0) < 0)
                {
                    my_error("recv", __LINE__);
                }

                //在chat目录下找到该帐号的主目录, 然后读取出密码文件
                itoa(logInfo.usernum, dirUserMain, 10);
                strcpy(tmpCurCwd, curCwd);
                strcat(tmpCurCwd, "chat/");
                strcat(tmpCurCwd, dirUserMain);
                strcpy(dirUserMain, tmpCurCwd);

                printf("###用户主目录为:%s\n", dirUserMain);

                if((dir = opendir(dirUserMain)) == NULL)
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
                            printf("dirent->d_name:%s\n", dirent->d_name);
                            strcpy(filePassword, dirUserMain);
                            strcat(filePassword, "/password.txt");
                            if((passwordFd = open(filePassword, O_RDWR)) < 0)
                            {
                                my_error("read", __LINE__);
                            }

                            fileLen = getFileLen(passwordFd);
                            if((ret = read(passwordFd, passwordBuf, fileLen)) < 0)
                            {
                                my_error("read", __LINE__);
                            }
                            printf("###密码为:%s\n", passwordBuf);
                            if(!strcmp(passwordBuf, logInfo.password))
                            {
                                getResult = SUCCESS;
                            }
                            break;
                        }
                    }
                }

                //读取结果用getResult发送
                //若帐号不存在则返回n, 若帐号存在, 密码不正确返回n
                if(send(conn_fd, &getResult, sizeof(getResult), 0) < 0)
                {
                    my_error("recv", __LINE__);
                }

                //登录成功之后, 将该客户端的信息储存在当前在线信息链表中
                addOnlineList(head, logInfo, conn_fd);
            }
            else if(oneMark == '2')
            {
                //标志位为2, 执行注册
                //先要将帐号发送过去

                if((sysNumberFd = open("chat/number.txt", O_RDWR)) < 0)
                {
                    my_error("open", __LINE__);
                } 

                fileLen = getFileLen(sysNumberFd);
                printf("fileLen:%d\n", fileLen);
                if((ret = read(sysNumberFd, numberBuf, fileLen)) < 0)
                {
                    my_error("read", __LINE__);
                }
                printf("buf:%s\n", numberBuf); 
                number = atoi(numberBuf);
                printf("number:%d\n", number);

                if(send(conn_fd, &number, sizeof(number), 0) < 0)
                {
                    my_error("send", __LINE__);
                }
                //number++;
                close(sysNumberFd);

                //需要接收一个REGINFO的结构体
                if(recv(conn_fd, &regInfo, sizeof(REGINFO), 0) < 0)
                {
                    my_error("recv", __LINE__);
                }
                printf("接收成功!\n");

                //printf("number :%d\n", regInfo.userinfo.usernum);
                //printf("username :%s\n", regInfo.userinfo.username);
                //printf("password :%s\n", regInfo.password);
                //printf("age :%d\n", regInfo.userinfo.age);
                //printf("sentence : %s\n", regInfo.userinfo.sentence);
                //printf("sex :%s\n", regInfo.userinfo.sex);
                //printf("month :%d\n", regInfo.userinfo.date.month);
                //printf("day :%d\n", regInfo.userinfo.date.day);
                //printf("constell :%s\n", regInfo.userinfo.constell);

                //对这个结构体进行一系列处理
                //然后建目录文件等等

                printf("创建文件之前\n");
                createDirAndFile(regInfo);

                //建好目录文件之后, 给客户端返回一个成功的信息
                getResult = SUCCESS;
                if(send(conn_fd, &getResult, sizeof(getResult), 0) < 0)
                {
                    my_error("send", __LINE__);
                }

                //再次把存有帐号的文件以清空的方式打开, 再将当前的系统帐号写进去
                number++;
                if((sysNumberFd = open("chat/number.txt", O_RDWR | O_TRUNC)) < 0)
                {
                    my_error("open", __LINE__);
                }
                itoa(number, numberBuf, 10);
                printf("一次注册之后number的值为:%s\n", numberBuf);
                if(write(sysNumberFd, numberBuf, strlen(numberBuf)) != strlen(numberBuf))
                {
                    my_error("write", __LINE__);
                }
                close(sysNumberFd);
            }
            else if(oneMark == '3')
            {
                //标志位为3, 执行接收消息
                //在正式对消息进行分类之前, 先给客户端发送一个含有当前登录的用户的信息的结构体,以便展示欢迎语
                printf("###接收到的标志位为3\n");
                itoa(logInfo.usernum, tmpUsernum, 10);
                printf("###用户信息的文件名为:%s\n", tmpUsernum);
                strcpy(tmpFileName, "chat/");
                strcat(tmpFileName, tmpUsernum);
                strcat(tmpFileName, "/userinfo.txt");
                printf("###用户信息的文件名为:%s\n", tmpFileName);
                if((userinfoFd = open(tmpFileName, O_RDWR)) < 0)
                {
                    my_error("open", __LINE__);
                }
                printf("资料文件已经打开\n");
                if((ret = read(userinfoFd, &userInfo, sizeof(USERINFO))) < 0)
                {
                    my_error("read", __LINE__);
                }
                printf("资料文件已经读出\n");
                close(userinfoFd);
                printf("资料文件已经关闭\n");
                //将读取到的用户信息发送给客户端
                if(send(conn_fd, &userInfo, sizeof(USERINFO), 0) < 0)
                {
                    my_error("send", __LINE__);
                }
                printf("资料文件已经发送给客户端\n");
                //好了, 现在要正式开始接收各种消息了
                //进行第二层标志位的判断
                printf("第二层while循环之前\n");
                while(1)
                {
                    mark = -1;
                    twoMark = 'z';
                    if((mark = recv(conn_fd, &twoMark, sizeof(twoMark), 0)) < 0)
                    {
                        my_error("recv", __LINE__);
                    }
                    else if(mark == 0)
                    {
                        pthread_exit(0);
                    }
                    else
                    {   
                        printf("第二层标志位为:%c\n", twoMark);
                        if(twoMark == 'u')
                        {
                            //第二层标志位为u, 则说明这是一个查看用户自身资料的消息
                            //此是要发送一个用户资料的结构体, 在之前已经得到过了, 所以现在就直接发过去
                            if(send(conn_fd, &userInfo, sizeof(USERINFO), 0) < 0)
                            {
                                my_error("send", __LINE__);
                            }
                        }
                        else if(twoMark == 'c')
                        {
                            //第二层标志位为c, 则说明要修改自身的资料
                            //要更改个人资料, 先要将用户当前的资料先发过去
                            if(send(conn_fd, &userInfo, sizeof(USERINFO), 0) < 0)
                            {
                                my_error("send", __LINE__);
                            }
                            //然后再接收改好的用户资料, 再清空用户资料文件, 然后再将改好的用户资料写进去
                            if(recv(conn_fd, &userInfo, sizeof(USERINFO), 0) < 0)
                            {
                                my_error("recv", __LINE__);
                            }
                            itoa(logInfo.usernum, tmpUsernum, 10);
                            printf("***用户信息的文件名为:%s\n", tmpUsernum);
                            strcpy(tmpFileName, "chat/");
                            strcat(tmpFileName, tmpUsernum);
                            strcat(tmpFileName, "/userinfo.txt");
                            printf("***用户信息的文件名为:%s\n", tmpFileName);
                            userInfo.usernum = logInfo.usernum;
                            if((userinfoFd = open(tmpFileName, O_RDWR | O_TRUNC)) < 0)
                            {
                                my_error("open", __LINE__);
                            }
                            if(write(userinfoFd, &userInfo, sizeof(USERINFO)) != sizeof(USERINFO))
                            {
                                my_error("write", __LINE__);
                            }
                            close(userinfoFd);
                        }
                        else if(twoMark == 'f')
                        {
                            //第二层标志位为f, 说明要查看好友, 好友又根据第三层标志位分为全部好友 在线好友 离线好友
                            if(threeMark == 'l')
                            {
                                //查看在线好友
                            }
                            else if(threeMark == 's')
                            {
                                //查看离线好友
                            }
                            else
                            {
                                //查看全部好友
                            }
                        }
                        else if(twoMark == 'a')
                        {
                            //第二层标志位为a, 说明要添加好友  
                        }
                        else if(twoMark == 'r')
                        {
                            //第二层标志位为r, 说明要删除好友
                        }
                        else if(twoMark == 'o')
                        {
                            //第二层标志位为o, 说明这是一个普通消息
                        }
                    }
                }
            }
        }
    }
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
