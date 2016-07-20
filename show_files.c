/*************************************************************************
	> File Name: show_files.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月20日 星期三 20时24分38秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<unistd.h>

int my_readdir(const char *name);

int my_readdir(const char *name)
{
    DIR *dir;
    struct dirent *dirent;

    if((dir = opendir(name)) == NULL)
    {
        perror("opendir");
        return -1;
    }
    
    while((dirent = readdir(dir)) != NULL)
    {
        printf("filename:%s\n", dirent->d_name);
    }
    
    closedir(dir);

    //之所以不用下面这段代码是因为:
    //在第一次对readdir()函数的返回值进行判断后,已经将该目录的第一个文件的信息错过了,
    //这样无法输出目录下的第一个文件的信息


    /*
    if((dirent = readdir(dir)) != NULL)
    {
        printf("filename:%s\n", dirent->d_name);
        while((dirent = readdir(dir)) != NULL)
        {
            printf("filename:%s\n", dirent->d_name);
        }
    }
    else
    {
        perror("readdir");
        return -1;
    }

    closedir(dir);

    */

    return 0;
}

int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        printf("please input:listfile <target path>\n");
        exit(1);
    }
    
    if(my_readdir(argv[1]) != 0)
    {
        exit(1);
    }

    return 0;
}
