/*************************************************************************
	> File Name: break_test.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月22日 星期五 15时38分10秒
 ************************************************************************/

#include<stdio.h>

int main(int argc,char *argv[])
{
    int i;
    int mark_one = 1;
    int mark_two = 1;

    for(i = 1; i < 11; i++)
    {
        if(mark_one)
        {
            if(mark_two)
            {
                //break;
                continue;
            }
        }

        printf("%d\n", i);
    }


    //在这个程序里,令mark_one和mark_two都等于1
    //不管if(mark_two)里面的是break还是continue,都不能输出1 2 3 ...
    //但是两者的情况是完全不同的
    //continue的情况:for循环仍然会循环10次,每一次都进到mark_two的循环里,然后跳出本层循环
    //               进行下一次循环,直至10次循环结束
    //break的情况:for循环第一次循环的时候进入到mark_two的循环里,直接break跳出所有的for循环,结束
}
