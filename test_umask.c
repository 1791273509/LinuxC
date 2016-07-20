/*************************************************************************
	> File Name: test_umask.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月20日 星期三 16时05分40秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
    umask(0);
    if((creat("example6.8.1.c", S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    {
        perror("creat");
        exit(1);
    }

    umask(S_IRWXO);
    if((creat("example6.8.2.c", S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    {
        perror("creat");
        exit(1);
    }

    if((creat("example6.8.3.c", S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
    {
        perror("creat");
        exit(1);
    }
    //umask(S_IRWXG);
    
    
    //关于umask()的用法测试总结:
    //1.umask()只能对在它之后创建的文件起作用,并且是对在它之后创建的所有文件起作用,不能对在它之前创建的文件起作用
    //2.umask()的参数都取反义,如0则为不屏蔽任何权限,而S_IRWXU | S_IRWXG | S_IRWXO则是屏蔽所有权限
}
