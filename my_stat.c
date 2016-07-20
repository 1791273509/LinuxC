/*************************************************************************
	> File Name: my_stat.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年07月20日 星期三 14时21分08秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>

int main(int argc,char *argv[])
{
    struct stat buf;

    //检查参数的个数
    if(argc != 2)
    {
        printf("please input:my_stat <filename>\n");
        exit(0);
    }
    

    if((stat(argv[1], &buf)) == -1)
    {
        perror("stat:");
        exit(1);
    }
    
    printf("device(文件的设备编号) is : %d\n", buf.st_dev);
    printf("i-node(文件的i-node编号) is : %d\n", buf.st_ino);
    printf("mode(文件的类型和存取权限) is : %c\n", buf.st_mode);
    printf("numbers of hard links(连到该文件的硬链接的个数) is :%d\n", buf.st_nlink);
    printf("user ID of owner(文件用户ID) is :%d\n", buf.st_uid);
    printf("group ID of owner(文件所有者的组ID) is : %d\n", buf.st_gid);
    printf("device type (if inode device)(如果这是一个设备文件,则为其设备编号) : %d\n", buf.st_rdev);
    printf("size, in byte(按字节计算文件的大小) is : %d\n", buf.st_size);
    printf("blocksize in byte(字节计算的文件系统的I/O缓冲区的大小) is : %d\n", buf.st_blksize);
    printf("numbers of blocks(占用文件区块的个数) : %d\n", buf.st_blocks);
    printf("time of last access(文件最后一次被访问的时间) : %s\n", ctime(&buf.st_atime));
    printf("time of last modification(文件最后一次被修改的时间) : %s\n", ctime(&buf.st_mtime));
    printf("time of last change(文件最后一次被更改的时间) : %s\n", ctime(&buf.st_ctime));

    return 0;
}
