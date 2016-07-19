/*************************************************************************
	> File Name: my_chmod.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月18日 星期一 17时34分47秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    int mode;                   //权限
    int mode_u;                 //user的权限
    int mode_g;                 //group的权限
    int mode_o;                 //other的权限
    char *path;                 //要更改权限的文件名

    //若输入的参数个数不够,则直接结束进程
    if(argc < 3)
    {
        printf("%s <mode num> <target file>\n", argv[0]);
        exit(0);
    }

    mode = atoi(argv[1]);

    if(mode > 777 || mode < 0)
    {
        printf("mode num is error!\n");
        exit(0);
    }

    mode_u = mode / 100;
    mode_g = (mode - mode_u * 100) / 10;
    mode_o = (mode_g - mode_g * 10);

    mode = mode_u * 8 * 8 + mode_g * 8 + mode_o;
    path = argv[2];

    if(chmod(path, mode) == -1)
    {
        perror("chmod error");
        exit(1);
    }

    return 0;
}
