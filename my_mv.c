/*************************************************************************
	> File Name: my_mv.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月20日 星期三 16时42分34秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        printf("please input:my_mv <oldfilename> <newdilename>\n");
        exit(1);
    }

    if(rename(argv[1], argv[2]) == -1)
    {
        perror("rename");
        exit(1);
    }

    return 0;
}
