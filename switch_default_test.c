/*************************************************************************
	> File Name: test.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月25日 星期一 11时32分03秒
 ************************************************************************/

#include<stdio.h>

int main(int argc,char *argv[])
{
    int n = 2;

    switch(n)
    {
        case 0 : printf("n = 0\n");
        case 1 : printf("n = 1\n");
        default : printf("n = other\n");
    }
}
