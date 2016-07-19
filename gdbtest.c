/*************************************************************************
	> File Name: gdbtest.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月18日 星期一 08时22分58秒
 ************************************************************************/

#include<stdio.h>

int get_sum(int n);

int get_sum(int n)
{
    int sum = 0;
    int i;

    for(i = 0; i < n; i++)
    {
        sum += i;
    }
    
    return sum;
}

int main(int argc,char *argv[])
{
    int i = 100;
    int result;

    result = get_sum(i);
    printf("1+2+3+...+%d = %d\n", i, result);

    return 0;
}
