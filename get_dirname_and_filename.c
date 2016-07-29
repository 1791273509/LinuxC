/*************************************************************************
	> File Name: test.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月29日 星期五 14时45分31秒
 ************************************************************************/

#include<stdio.h>
#include<libgen.h>
#include<unistd.h>
#include<string.h>

int main(int argc,char *argv[])
{
    char path[4096] = {0};
    char filename[256] = {0};
    char thisDirname[4096] = {0};

    strcpy(path, getcwd(NULL, 0));
    strcpy(filename, basename(path));
    strcpy(thisDirname, dirname(path));
    printf("%s\n", filename);
    printf("%s\n", thisDirname);
    
    return 0;
    //这是两个好用到哭的函数!!
    //通过getcwd()得到当前路径path,然后通过这两个函数,就能分别得到当前的文件名和目录名!!
}
