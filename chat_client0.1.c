/*************************************************************************
	> File Name: chat_server.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: Thu 11 Aug 2016 09:51:30 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include <gtk/gtk.h>

#define ONLINE          'l'
#define STEALTH         's'

typedef struct DATE
{
    int month;
    int day;
}DATE;

typedef struct FRIENDINFO
{
    int usernum;
    char name[20];
}FRIENDINFO;

typedef struct USERINFO
{
    int usernum;
    char username[20];      //注册时, 昵称不能超过八个字
    char sex[4];            //年龄
    int age;                //性别
    DATE day;               //生日
    char constell[8];       //星座
    char sentence[40];      //个性签名, 不超过十五个字, 设置个性签名时应该检查字数
}USERINFO;

typedef struct REGINFO
{
    char password[17];          //不超过16位的密码
    USERINFO userinfo;          //用户基本信息
    FRIENDINFO *friendlist;     //好友列表
}REGINFO;

typedef struct LOGINFO
{
    int usernum;
    char password[17];
    char state;             //状态
}LOGINFO;

static GtkWidget *entry1;
static GtkWidget *entry2;
int number = 16081101;      //由系统统一分配的帐号

void my_error(const char *err_string, int line);
void my_gtk_main(int argc, char *argv[]);
void on_button_clicked(GtkWidget *button, gpointer data);

void client_main(int argc, char *argv[]);

void client_main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;       //服务器地址结构
    int conn_fd;                        //连接套接字
    int i;

    
    //检查参数个数, 参数个数应为3, 不是3, 则直接退出
    if(argc != 5)
    {
        printf("Usage:[-a] [serv_address]\n");
        exit(1);
    }

    //初始化服务器地址结构
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;     //初始化地址类型
    serv_addr.sin_port = 4507;          //初始化端口号

    //从命令行获取服务器端的地址
    for(i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "-a"))
        {
            if(inet_aton(argv[i+1], &serv_addr.sin_addr) == 0)
            {
                printf("invalid server ip address\n");
                exit(1);
            }
            continue;
        }
    }

    //创建一个TCP套接字
    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0)
    {
        my_error("socket", __LINE__);
    }

    //向服务器端发送连接请求

    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
    {
        my_error("connect", __LINE__);
    }


    //进入登录界面, 输入用户名和密码, 若用户名不存在返回一个提示信息
    

}

void on_button_clicked(GtkWidget *button, gpointer data)
{
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry1));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry2));

    g_print("用户名是:%s\n", username);
    g_print("密码是:%s\n", password);
}


void my_gtk_main(int argc, char *argv[])
{
    GtkWidget *window;      //窗口
    GtkWidget *box;         //含有用户名盒子及密码盒子的大盒子
    GtkWidget *box1;        //用户名标签及编辑框盒子
    GtkWidget *box2;        //密码标签及编辑框盒子
    GtkWidget *label1;      //登录标签
    GtkWidget *label2;      //注册标签
    GtkWidget *button1;     //登录按钮
    GtkWidget *button2;     //注册按钮
    GtkWidget *sep;         //设置间距
    GtkWidget *image;       //登录界面的图片

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "登录窗口");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    box1 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), box1, FALSE, FALSE, 5);
    
    box2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), box2, FALSE, FALSE, 5);

    sep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(box), sep, FALSE, FALSE, 5);

    label1 = gtk_label_new("帐号");
    entry1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box1), label1, FALSE, FALSE, 50);
    gtk_box_pack_start(GTK_BOX(box1), entry1, FALSE, FALSE, 5);
    
    label2 = gtk_label_new("密码");
    entry2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry2), FALSE);     //第二个参数为false可保证输入的密码显示为*
    gtk_box_pack_start(GTK_BOX(box2), label2, FALSE, FALSE, 50);
    gtk_box_pack_start(GTK_BOX(box2), entry2, FALSE, FALSE, 5);

    button1 = gtk_button_new_with_label("登录");
    g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(on_button_clicked), NULL);//此处应自定义登录函数
    g_signal_connect_swapped(G_OBJECT(button1), "clicked", G_CALLBACK(gtk_main_quit), window);
    gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE, 5);
    
    button2 = gtk_button_new_with_label("注册");
    g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(on_button_clicked), NULL);//此处应自定义注册函数
    g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(gtk_main_quit), window);
    gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 5);

    image = gtk_image_new_from_file("login.png");
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 3);
    
    gtk_widget_show_all(window);
    gtk_main();
}

void my_error(const char *err_string, int line)
{
    fprintf(stderr, "line:%d ", line);
    perror(err_string);
    exit(1);
}

int main(int argc,char *argv[])
{
    my_gtk_main(argc, argv);
}
