/*************************************************************************
	> File Name: test.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月22日 星期五 14时10分27秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
    char buf[100] = {0};
    char *result;


    result = getcwd(buf, 100);

    printf("%s\n", buf);
    printf("%s\n", result);

    //第一种用法
    //buf的结果是/home/dela/dela_c
    //result的结果是/home/dela/dela_c
    //buf的地址是0x7fffffffde20
    //result的地址是0x7fffffffde20
    

    char *result2;

    result2 = getcwd(NULL, 0);
    
    printf("%s\n", result2);
    
    //第二种用法
    //result2的结果是/home/dela/dela_c
    
    //以后用函数 一定要搞清楚它的用法 切记切记

}
