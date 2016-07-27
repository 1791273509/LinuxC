/*************************************************************************
	> File Name: myshell.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月27日 星期三 10时34分25秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<dirent.h>
#include<pwd.h>
#include<fcntl.h>

#define NO_SYMBOL    0
#define HAVE_BACK    1
#define HAVE_INPUT   2
#define HAVE_OUT     3
#define HAVE_PIPE    4

void shell_main(int argc, char *argv[]);
void *Malloc(size_t size);
void printShellPrompt();
void inputCommand(char *buf);
int explainCommand(char *buf, char argumentList[100][256]);
void backgroundSymbol();
void inputRedirectionSymbol();
void outputRedirectionSymbol();
void pipeSymbol();
void executeCommand();
int isCommandExisted(char *command);
int judgeSymbolType(char *buf);

int judgeSymbolType(char *buf)
{
    int backMark = 0;
    int inputMark = 0;
    int outputMark = 0;
    int pipeMark = 0;
    int i;

    for(i = 0; buf[i] != '\0'; i++)
    {
        if(buf[i] == '&')
        {
            backMark++;
        }
        else if(buf[i] == '>')
        {
            inputMark++;
        }
        else if(buf[i] == '<')
        {
            outputMark++;
        }
        else if(buf[i] == '|')
        {
            pipeMark++;
        }
    }
    
    if(backMark == 1)
    {
        type = HAVE_BACK;
        return type;
    }
    else if(inputMark == 1)
    {
        type = HAVE_INPUT;
        return type;
    }
    else if(outputMark == 1)
    {
        type = HAVE_OUT;
        return type;
    }
    else if(pipeMark == 1)
    {
        type = HAVE_PIPE;
        return type;
    }
    else
    {
        type = NO_SYMBOL;
        return type;
    }
}

int isCommandExisted(char *command)
{
    char *path[] = {"./", "/bin", "/usr/bin", NULL};
    DIR *dir;
    struct dirent *dirent;
    int i = 0;

    //在几个命令下查询命令是否存在
    //存在返回1
    //不存在返回0
   
    if(!strncmp(command, "./", 2))          //这里是为了判断./a.out这类命令
    {                                       //即要判断a.out这个文件是否存在于当前目录下
        command = command + 2;              //所有就要把前面的./去掉后再判断
    }

    while(path[i])
    {
        if((dir = opendir(path[i])) == NULL)
        {
            printf("can not open directory\n");
        }
        while((dirent = readdir(dir)) != NULL)
        {
            if(!strcmp(dirent->d_name, command))
            {
                closedir(dir);
                return 1;
            }
        }
        closedir(dir);
        i++;
    }

    return 0;
}

void executeCommand()
{
    //执行处理好的命令

    //处理好之后查询命令是否存在
    isCommandExisted();
}

void pipeSymbol()
{
    //处理有管道符的命令
    //处理好之后查询命令是否存在
    isCommandExisted();
}

void outputRedirectionSymbol()
{
    //处理有输出重定向的命令
    //处理好之后查询命令是否存在
    isCommandExisted();
}

void inputRedirectionSymbol()
{
    //处理有输入重定向符的命令
    //处理好之后查询命令是否存在
    isCommandExisted();
}

void backgroundSymbol()
{
    //处理有后台符的命令
    //处理好之后查询命令是否存在
    isCommandExisted();
}

int explainCommand(char *buf, char argumentList[100][256])
{
    int bufIndex = 0;
    int argcount;
    int i;
    int j;

    //解释输入的命令
    
    for(i = 0; buf[bufIndex] != '\0'; i++)
    {
        for(j = 0; buf[bufIndex] != ' ' && buf[bufIndex] != '\0'; j++)
        {
            argumentList[i][j] = buf[bufIndex];
            bufIndex++;
        }
        bufIndex++;
    }
    argcount = i;

    return argcount;
}

void inputCommand(char *buf)
{
    char ch;
    int len = 0;

    //input函数中不但要输入命令, 还要将命令分割开,处理好

    ch = getchar();
    while(len < 256 || ch != '\n')
    {
        buf[len++] = ch;
        ch = getchar();
    }

    if(len == 256)
    {
        printf("command is too long\n");
        exit(-1);
    }
}

void printShellPrompt()
{
    char *path;
    char tmp[256] = {0};
    char dirname[256] = {0};
    pid_t uid;
    int mark;
    int len;
    int i;
    int j;

    //获取当前路径,将当前路径存在path中,然后把path接到提示符之后
    //再判断当前的用户,如果是root用户则用#,否则用$
    //输出shell提示符
    
    path = getcwd(NULL, 0);
    len = strlen(path);
    
    if(len == 1)
    {
        dirname[0] = '/';
    }
    else
    {
        for(i = len - 1; path[i] != '/'; i--)
        {
            tmp[j++] = path[i];
        }

        for(i = 0; tmp[i]; i++)
        {
            dirname[j-i-1] = tmp[i];
        }
    }

    uid = getuid();
    if(uid == 0)
    {
        mark = '#';
    }
    else
    {
        mark = '$';
    }

    printf("[%s@server1 %s]%c", userName, dirname, mark);    
}

void *Malloc(size_t size)
{
    void *result;

    result = malloc(size);

    if(NULL == result)
    {
        printf("内存分配失败!\n");
        exit(-1);
    }

    return result;
}

void shell_main(int argc, char *argv[])
{
    char *buf;
    char argumentList[100][256];
    int argcount;
    int type;

    buf = (char *)Malloc(sizeof(char) * 256);

    while(1)
    {
        //先打印shell提示符, 提示符根据路径名的改变而改变
        printShellPrompt();

        //等待输入要执行的命令
        //用input函数, input函数主要用于输入命令, 并且处理输入的命令
        inputCommand(buf);

        //判度输入的是不是退出命令
        if(!strcmp(buf, "exit\n") || !strcmp(buf, "logout\n"))
        {
            break;
        }

        if(/*输入的是cd*/)
        {
            //进行cd命令的一系列处理
        }
        else
        {
            //解释处理输入的命令
            argcount = explainCommand(buf, argumentList);
            //判断命令里面有没有后台符, 输入重定向符, 输出重定向符, 管道符,
            //如果有后台符, 输入重定向符, 输出重定向符, 管道符
            //根据记号将他们分开, 再分别编写函数处理
            
            type = judgeSymbolType(buf);
            if(type == HAVE_BACK)
            {
                backgroundSymbol();
                executeCommand();
            }
            else if(type == HAVE_INPUT)
            {
                inputRedirectionSymbol();
                executeCommand();
            }
            else if(type == HAVE_OUT)
            {
                outputRedirectionSymbol();
                executeCommand();
            }
            else if(type == HAVE_PIPE)
            {
                pipeSymbol();
                executeCommand();
            }
            else if(type == NO_SYMBOL) //如果没有以上这些符号,则当作普通命令执行
            {
                isCommandExisted();
                executeCommand();
            }
        }
    }

    //如果输入了exit或logout, 则释放内存, 并结束进程
    if(buf)
    {
        free(buf);
        buf = NULL;
    }

    exit(0);
}

int main(int argc,char *argv[])
{
    shell_main(argc, argv);
}
