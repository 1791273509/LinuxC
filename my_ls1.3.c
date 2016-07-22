/*************************************************************************
	> File Name: my_ls.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月22日 星期五 12时38分45秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<malloc.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>
#include<time.h>

#define HAVE_A_ARGUMENT   1
#define NO_A_ARGUMENT     0

typedef struct File
{
    char type;
    char mode[10];
    int number;
    char uid[20];
    char gid[20];
    off_t size;
    time_t mtime;
    char filename[30];
}File;

File * fileinfo;
int filecount;

void my_err(const char *err_string, int line);
void ls_main(int argc, char *argv[]);
void getDirMaxlenAndCount(char dirname[]);          //得到目录文件的总数和最长文件名 和文件的各项信息
void getDirFileInfo(char dirname[]);
void judgmentArguments(char argument[]);
void displayFileInfoLong(int amark);
void displayFileInfo(int amark);

void displayFileInfo(int amark)
{
    int i;
    i = (amark == HAVE_A_ARGUMENT) ? 0 : 2;

    for(; i < filecount; i++)
    {
        printf("%s\n", fileinfo[i].filename);
    }
}

void displayFileInfoLong(int amark)
{
    int i;
    
    i = (amark == HAVE_A_ARGUMENT) ? 0 : 2;

    for(; i < filecount; i++)
    {
        printf("%c%s %d %s %s %d %s %s\n", fileinfo[i].type, fileinfo[i].mode, fileinfo[i].number, fileinfo[i].uid, fileinfo[i].gid, fileinfo[i].size, ctime(&fileinfo[i].mtime), fileinfo[i].filename);
    }
}

void judgmentArguments(char argument[])
{
    int amark;

    if(!strcmp(argument, "-l")) 
    {
        //以长清单的形式从第三个文件开始显示
        amark = NO_A_ARGUMENT;
        displayFileInfoLong(amark);
    }
    else if(!strcmp(argument, "-a"))
    {
        //从第一个文件开始只显示文件名
        amark = HAVE_A_ARGUMENT;
        displayFileInfo(amark);
    }
    else if(!strcmp(argument, "-al") || !strcmp(argument, "-la"))
    {
        //以长清单的形式从第一个文件开始显示
        amark = HAVE_A_ARGUMENT;
        displayFileInfoLong(amark);
    }
}

void getDirFileInfo(char dirname[])
{
    DIR *dir;
    struct dirent *dirent;
    int i = 0;
    struct stat *buf;
    struct passwd *usr;
    struct group *grp;
    char filename[80] = {0};
    
    buf = (struct stat *)malloc(sizeof(struct stat));

    if((dir = opendir(dirname)) == NULL)
    {
        my_err("opendir", __LINE__);
    }



    while((dirent = readdir(dir)) != NULL)
    {
        strcpy(filename, dirname);
        strcat(filename, "/");
        strcat(filename, dirent->d_name);


        lstat(filename, buf);                  //用lstat避免奇奇怪怪的错误

        //获取文件类型
        if(S_ISLNK(buf->st_mode))
        {
            fileinfo[i].type = 'l';
        }
        else if(S_ISREG(buf->st_mode)) 
        {
            fileinfo[i].type = '-';
        }
        else if(S_ISDIR(buf->st_mode))
        {
            fileinfo[i].type = 'd';
        }
        else if(S_ISCHR(buf->st_mode))
        {
            fileinfo[i].type = 'c';
        }
        else if(S_ISBLK(buf->st_mode))
        {
            fileinfo[i].type = 'b';
        }
        else if(S_ISFIFO(buf->st_mode))
        {
            fileinfo[i].type = 'f';
        }
        else if(S_ISSOCK(buf->st_mode))
        {
            fileinfo[i].type = 's';
        }

        //获取文件权限
        fileinfo[i].mode[0] = (buf->st_mode & S_IRUSR) ? 'r' : '-';
        fileinfo[i].mode[1] = (buf->st_mode & S_IWUSR) ? 'w' : '-';
        fileinfo[i].mode[2] = (buf->st_mode & S_IXUSR) ? 'x' : '-';
        fileinfo[i].mode[3] = (buf->st_mode & S_IRGRP) ? 'r' : '-';
        fileinfo[i].mode[4] = (buf->st_mode & S_IWGRP) ? 'w' : '-';
        fileinfo[i].mode[5] = (buf->st_mode & S_IXGRP) ? 'x' : '-';
        fileinfo[i].mode[6] = (buf->st_mode & S_IROTH) ? 'r' : '-';
        fileinfo[i].mode[7] = (buf->st_mode & S_IWOTH) ? 'w' : '-';
        fileinfo[i].mode[8] = (buf->st_mode & S_IXOTH) ? 'x' : '-';

        printf("%s\n", fileinfo[i].mode);

        //获取文件硬链接的个数
        fileinfo[i].number = buf->st_nlink;
        
        //获取文件的所有者
        usr = getpwuid(buf->st_uid);
        strcpy(fileinfo[i].uid, usr->pw_name);

        //获取文件所有者的所属组
        grp = getgrgid(buf->st_gid);
        strcpy(fileinfo[i].gid, grp->gr_name);

        //获取文件的大小
        fileinfo[i].size = buf->st_size;

        //获取文件最后更改的时间
        fileinfo[i].mtime = buf->st_mtime;

        //获取文件的文件名
        strcpy(fileinfo[i].filename, dirent->d_name);
        
        i++;
    }
}


void getDirMaxlenAndCount(char dirname[])
{
    DIR *dir;
    struct dirent *dirent;
    int maxFilenameSize = 0;

    filecount = 0;
    if((dir = opendir(dirname)) == NULL)
    {
        my_err("opendir", __LINE__);
    }
    
     while((dirent = readdir(dir)) != NULL)
    {
        if(maxFilenameSize < strlen(dirent->d_name))
        {
            maxFilenameSize = strlen(dirent->d_name);
        }
        filecount += 1;
    }
    closedir(dir);

    fileinfo = (File *)malloc(sizeof(File) * filecount);
}

void ls_main(int argc, char *argv[])
{
    char *dirname;
    char *argument;
    int filecount;
    int nummark;

    if(argc > 3)
    {
        printf("please input:my_ls [argument] [directory]\n");
        exit(1);
    }
    else if(argc == 1)
    {
        //获取当前目录 dirname = 当前目录
        dirname = NULL;
        nummark = NO_A_ARGUMENT;
        
        dirname = getcwd(NULL, 0);                 //令dirname = 当前目录
        getDirMaxlenAndCount(dirname);
        getDirFileInfo(dirname);
        displayFileInfo(nummark);

       // free(dirname);

        //并显示除了. .. 之外的文件名称
    }
    else if(argc == 2)
    {
        //令一个字符串tmp=argv[1]
        argument = argv[1];

        if(argv[1][0] == '-')
        {
            //判断tmp的三种情况
            
            dirname = getcwd(NULL, 0);
            getDirMaxlenAndCount(dirname);
            getDirFileInfo(dirname);
            judgmentArguments(argument);
        }
        else
        {
            //获取argv[1]的目录 dirname = 指定目录
            nummark = NO_A_ARGUMENT;
            dirname = argv[1];
            getDirMaxlenAndCount(dirname);
            getDirFileInfo(dirname);
            displayFileInfo(nummark);
        }
    }
    else if(argc == 3)
    {
        //先获取argv[2]的目录内容
        dirname = argv[2];
        getDirMaxlenAndCount(dirname);
        getDirFileInfo(dirname);


        //再令tmp = argv[1]
        argument = argv[1];

        //判断tmp的三种情况
        judgmentArguments(argument);

    }
}

void my_err(const char *err_string, int line)
{
    fprintf(stderr, "line:%d  ", line);
    perror(err_string);
    exit(1);
}

int main(int argc,char *argv[])
{
    ls_main(argc, argv);


    //已经把问题1 3 4 解决 1.3版
}
