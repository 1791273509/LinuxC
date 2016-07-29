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
void backgroundSymbol(char *argument[]);
void inputRedirectionSymbol(char *argument[]);
void outputRedirectionSymbol(char *argument[]);
void pipeSymbol(char *argument[], int argcount);
int isCommandExisted(char *command);
int judgeSymbolType(char *buf);
void noSymbol(char *argument[]);
void cdCommand(int argcount, char *argument[]);

void cdCommand(int argcount, char *argument[])
{
    char path[PATH_MAX+1] = {0};
    struct passwd *usr;
    pid_t uid;
    char curPath[PATH_MAX] = {0};
    int len;
    int i;

    uid = getuid();
    usr = getpwuid(uid);
    
    memset(curPath, 0, PATH_MAX);
    memset(path, 0, PATH_MAX);

    strcpy(curPath, getcwd(NULL, 0));
    len = strlen(curPath);
    
    if(curPath[len-1] != '/')
    {
        strcat(curPath, "/");
    }
    
    if(argcount == 1 || !strcmp(argument[1], "~"))
    {
        strcpy(path, usr->pw_dir);
    }
    else if(!strncmp(argument[1], "/", 1))
    {
        strcpy(path, argument[1]);
    }
    else if(!strncmp(argument[1], "..", 2))
    {
        for(i = len - 2; curPath[i] != '/'; i--)
        {
            curPath[i] = '\0';
        }
        if(strlen(curPath) != 1)
        {
            curPath[i] = '\0';
        }
        strcpy(path, curPath);
    }
    else if(!strncmp(argument[1], ".", 1))
    {
        argument[1] = argument[1] + 1;
        strcat(curPath, argument[1]);
        strcpy(path, curPath);
    }
    else
    {
        strcpy(path, curPath);
        strcat(path, argument[1]);
    }
    
    if(chdir(path) < 0)
    {
        printf("cd error\n");
        
        return;
    }
}

void noSymbol(char *argument[])
{
    pid_t pid;
    int stat_val;

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    }

    if(pid == 0)
    {
        if(!isCommandExisted(argument[0]))
        {
            printf("%s : command not found\n", argument[0]);
            exit(0);
        }

        execvp(argument[0], argument);
        exit(0);
    }
    else
    {
        if(waitpid(pid, &stat_val, 0) == -1)
        {
            printf("wait for child process error\n");
        }
    }
}

int judgeSymbolType(char *buf)
{
    int backMark = 0;
    int inputMark = 0;
    int outputMark = 0;
    int pipeMark = 0;
    int type;
    int i;

    for(i = 0; buf[i] != '\0'; i++)
    {
        if(buf[i] == '&')
        {
            backMark++;
        }
        else if(buf[i] == '>')
        {
            outputMark++;
        }
        else if(buf[i] == '<')
        {
            inputMark++;
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
    //这里是为了判断./a.out这类命令
    if(!strncmp(command, "./", 2))         
    {  
        //即要判断a.out这个文件是否存在于当前目录下                                     
        command = command + 2;             
        //所有就要把前面的./去掉后再判断
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

void pipeSymbol(char *argument[], int argcount)
{
    pid_t pid;
    pid_t sonPid;
    int stat_val;
    int sonStat_val;
    int sonFd;
    char *behindArgument[argcount+1];
    int i;
    int j;

    //处理有管道符的命令
    //处理好之后查询命令是否存在

    for(i = 0; argument[i] != NULL; i++)
    {
        if(!strcmp(argument[i], "|"))
        {
            argument[i] = NULL;

            for(j = i+1; argument[j] != NULL; j++)
            {
                behindArgument[j-i-1] = argument[j];
            }
            behindArgument[j-i-1] = argument[j];
        }
    }

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    }
    else
    {
        int status;
        if(pid > 0)
        {
            waitpid(pid, &stat_val, 0);
        }
        else if(pid == 0)
        {
            if((sonPid = fork()) < 0)
            {
                printf("fork error\n");
                exit(-1);
            }
            else 
            {
                if(sonPid == 0)
                {
                    if(!isCommandExisted(argument[0]))
                    {
                        printf("%s : command not found\n", argument[0]);
                        exit(0);
                    }

                    if((sonFd = open("/tmp/tmpfile", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
                    {
                        printf("open file error\n");
                        exit(-1);
                    }

                    //sonFd = open("/tmp/tmpfile", O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    dup2(sonFd, 1);
                    execvp(argument[0], argument);
                    exit(0);
                }
            }

            if(waitpid(sonPid, &sonStat_val, 0) == -1)
            {
                printf("wait for child process error\n");
            }

            if(!isCommandExisted(behindArgument[0]))
            {
                printf("%s : command not found\n", behindArgument[0]);
                exit(0);
            }

            if((sonFd = open("/tmp/tmpfile", O_RDONLY)) == -1)
            {
                printf("open file error\n");
                exit(-1);
            }

            dup2(sonFd, 0);
            execvp(behindArgument[0], behindArgument);

            if(remove("/tmp/tmpfile"))
            {
                printf("remove error\n");
                exit(-1);
            }
            exit(0);
        }
    }
}

void outputRedirectionSymbol(char *argument[])
{
    pid_t pid;
    int oldfd;
    char *file;
    int i;
    int stat_val;

    //处理有输出重定向的命令
    //处理好之后查询命令是否存在

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    }
    else if(pid == 0)
    {
        if(!isCommandExisted(argument[0]))
        {
            printf("%s : command not found\n", argument[0]);
            exit(0);
        }

        for(i = 0; argument[i] != NULL; i++)
        {
            if(!strcmp(argument[i], ">"))
            {
                file = argument[i+1];
                argument[i] = NULL;
            }
        }

        if((oldfd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1)
        {
            printf("open file error\n");
            exit(-1);
        }

        dup2(oldfd, 0);
        execvp(argument[0], argument);
        exit(0);
    }
    else if(pid > 0)
    {
        if(waitpid(pid, &stat_val, 0) == -1)
        {
            printf("wait for child process error\n");
        }
    }
}

void inputRedirectionSymbol(char *argument[])
{
    pid_t pid;
    int oldfd;
    char *file;
    int stat_val;
    int i;

    //处理有输入重定向符的命令
    //处理好之后查询命令是否存在

    for(i = 0; argument[i] != NULL; i++)
    {
        if(!strcmp(argument[i], "<"))
        {
            file = argument[i+1];
            argument[i] = NULL;
        }
    }

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    }
    else if(pid == 0)
    {
        if(!isCommandExisted(argument[0])) 
        {
            printf("%s : command not found\n", argument[0]);
            exit(0);
        }

        if((oldfd = open(file, O_RDONLY)) == -1)
        {
            printf("open file error\n");
            exit(-1);
        }

        dup2(oldfd, 1);
        execvp(argument[0], argument);
        exit(0);
    }
    else if(pid > 0)
    {
        if(waitpid(pid, &stat_val, 0) == -1)
        {
            printf("wait for child process error\n");
        }
    }
}

void backgroundSymbol(char *argument[])
{
    pid_t pid;
    int stat_val;
    //处理有后台符的命令
    //处理好之后查询命令是否存在

    if((pid = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    }

    printf("[process id is %d]\n", pid);
    
    return;
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
    while(len < 256 && ch != '\n')
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
    char path[PATH_MAX + 1] = {0};
    char tmp[256] = {0};
    char dirname[256] = {0};
    struct passwd *usr;
    char userName[80];
    pid_t uid;
    int mark;
    int len;
    int i;
    int j = 0;

    //获取当前路径,将当前路径存在path中,然后把path接到提示符之后
    //再判断当前的用户,如果是root用户则用#,否则用$
    //输出shell提示符

    strcpy(path, getcwd(NULL, 0));
    len = strlen(path);


    uid = getuid();
    usr = getpwuid(uid);
    strcpy(userName, usr->pw_name);

    if(len == 1)
    {
        dirname[0] = '/';
    }
    else if(!strcmp(path, usr->pw_dir))
    {
        strcpy(dirname, "~");
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

    if(uid == 0)
    {
        mark = '#';
    }
    else
    {
        mark = '$';
    }

    printf("[%s@server2 %s]%c ", userName, dirname, mark);    
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
    char argumentList[100][256] = {0};
    int argcount;
    int type;
    int i;

    buf = (char *)Malloc(sizeof(char) * 256);
    while(1)
    {
        memset(buf, 0, 256);
        for(i = 0; i < 100; i++)
        {
            memset(argumentList[i], 0, 256);
        }
        //先打印shell提示符, 提示符根据路径名的改变而改变
        printShellPrompt();

        //等待输入要执行的命令
        //用input函数, input函数主要用于输入命令, 并且处理输入的命令
        inputCommand(buf);

 	    argcount = explainCommand(buf, argumentList);
           
        char *argument[argcount + 1];
        
        for(i = 0; i < argcount; i++)
        {
            argument[i] = (char *)argumentList[i];
        }
        argument[argcount] = NULL;

        //判度输入的是不是退出命令
        if(!strcmp(buf, "exit") || !strcmp(buf, "logout"))
        {
            break;
        }

        if(!strncmp(buf, "cd", 2))
        {
            //进行cd命令的一系列处理
            //argcount = explainCommand(buf, argumentList);
            cdCommand(argcount, argument);
        }
        else
        {
            type = judgeSymbolType(buf);
            if(type == HAVE_BACK)
            {
                backgroundSymbol(argument);
            }
            else if(type == HAVE_INPUT)
            {
                inputRedirectionSymbol(argument);
            }
            else if(type == HAVE_OUT)
            {
                outputRedirectionSymbol(argument);
            }
            else if(type == HAVE_PIPE)
            {
                pipeSymbol(argument, argcount);
            }
            else if(type == NO_SYMBOL)
            //如果没有以上这些符号,则当作普通命令执行
            {
                noSymbol(argument);
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
