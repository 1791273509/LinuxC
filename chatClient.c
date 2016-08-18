/*************************************************************************
	> File Name: chatClient.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: Wed 17 Aug 2016 09:03:03 PM CST
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
#include<pthread.h>

#define BUFSIZE     1024

#define SUCCESS     'y'
#define FAIL        'n'

#define ONLINE      'l'
#define STEALTH     's'

#define LOG         '1'
#define REG         '2'

typedef struct DATE         //日期结构体
{
    int month;
    int day;
}DATE;

typedef struct FRIENDINFO   //好友列表结构体
{
    int usernum;
    char name[20];
}FRIENDINFO;

typedef struct USERINFO     //用户基本信息结构体
{
    int usernum;
    char username[20];      //注册时, 昵称不能超过八个字
    char sex[4];            //年龄
    int age;                //性别
    DATE date;              //生日
    char constell[10];      //星座
    char sentence[40];      //个性签名, 不超过十五个字, 设置个性签名时应该检查字数
}USERINFO;

typedef struct REGINFO              //用户注册信息结构体
{
    char password[17];              //不超过16位的密码
    USERINFO userinfo;              //用户基本信息
    FRIENDINFO friendlist[200];     //好友列表
}REGINFO;

typedef struct LOGINFO      //用户登录结构体
{
    int usernum;
    char password[17];
    char state;             //状态
}LOGINFO;

typedef struct MESSAGE
{
    FRIENDINFO friendinfo;  //好友的信息, 包含好友的帐号, 好友的名字, 根据好友帐号查询他的conn_fd, 名字提供聊天框信息
    char messagecontent[BUFSIZE];   //消息内容  
}MESSAGE;


typedef struct ALL
{
    char mark;              //标志数据类型
    REGINFO reginfo;        //注册结构体
    LOGINFO loginfo;        //登录结构体
    USERINFO userinfo;      //存储用户个人信息的结构体
    USERINFO friendallinfo; //存储好友的个人完整信息的结构体
    MESSAGE message;
}ALL;

int conn_fd; 
int number;
char username[20];
char getResult = FAIL;
char logFail;
GtkWidget *messageWindow;
static GtkWidget *entry1;
static GtkWidget *entry2;

void my_error(const char *err_string, int line);
void itoa(unsigned long val, char *buf, unsigned radix);
void client_main(int argc, char *argv[]);
void client_init(int argc, char *argv[]);
void loginInterface(int argc, char *argv[]);
char aboutState(GtkWidget *widget, gpointer data);
void loginFunction(GtkWidget *widget, gpointer data);
void messagePromptInterface(int argc, char *argv[]);
void registeredInterface(GtkWidget *widget, gpointer data, int argc, char *argv[]);
void registeredFunction(GtkWidget *widget, gpointer *entry[], int argc, char *argv[]);
void mainInterface(int argc, char *argv[]);
void client_recv();
void dealMain(ALL messageAll);
void showInfoFunction();
void changedUserinfoFunction();

void changedUserinfoFunction()
{

}

void showInfoFunction()
{

}

void dealMain(ALL messageAll)
{
	
}

void client_recv()
{
    ALL messageAll;

    while(1)
    {
        if(recv(conn_fd, &messageAll, sizeof(messageAll), 0) < 0)
        {
            my_error("recv", __LINE__);
        }

        dealMain(messageAll);
    }
}

void mainInterface(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *bigBox;
    GtkWidget *box[9];
    GtkWidget *button[7];
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *image;
    GtkWidget *image1;
    GtkWidget *label;
    GtkWidget *sep;
    char oneMark;
    int i;
    char welcome[50];

    strcpy(welcome, "hello, ");
    strcat(welcome, username);
    strcat(welcome, ", 欢迎回来!");
    
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "主界面");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 50);

    bigBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), bigBox);

    label= gtk_label_new(welcome);
    gtk_box_pack_start(GTK_BOX(bigBox), label, FALSE, FALSE, 0);
    sep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(bigBox), sep, FALSE, FALSE, 10);

    //image1 = gtk_image_new_from_file("main1.jpg");
    //gtk_box_pack_start(GTK_BOX(bigBox), image1, FALSE, FALSE, 0);
    for(i = 0; i < 9; i++)
    {
        box[i] = gtk_vbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(bigBox), box[i], FALSE, FALSE, 0);
        printf("第%d个盒子放置成功!\n", i+1);
    }

    button[0] = gtk_button_new_with_label("查看我的资料");
    g_signal_connect(G_OBJECT(button[0]), "clicked", G_CALLBACK(showInfoFunction), NULL);
    //g_signal_connect_swapped(G_OBJECT(button[0]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box[0]), button[0], FALSE, FALSE, 5);
    //sep = gtk_hseparator_new();
    //gtk_box_pack_start(GTK_BOX(box[0]), sep, FALSE, FALSE, ;
    //gtk_widget_set_size_request(box[0], 150, 40);


    button[1] = gtk_button_new_with_label("修改我的资料");
    g_signal_connect(G_OBJECT(button[1]), "clicked", G_CALLBACK(changedUserinfoFunction), NULL);
    //g_signal_connect_swapped(G_OBJECT(button[1]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box[1]), button[1], FALSE, FALSE, 5);
    //sep = gtk_hseparator_new();
    //gtk_box_pack_start(GTK_BOX(box[1]), sep, FALSE, FALSE, 3);
    //gtk_widget_set_size_request(box[1], 150, 40);

    button[2] = gtk_button_new_with_label("   查看好友   ");
    g_signal_connect(G_OBJECT(button[2]), "clicked", G_CALLBACK(gtk_main_quit), "2");
    //g_signal_connect_swapped(G_OBJECT(button[2]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box[2]), button[2], FALSE, FALSE, 5);
    //sep = gtk_hseparator_new();
    //gtk_box_pack_start(GTK_BOX(box[2]), sep, FALSE, FALSE, 10);
    //gtk_widget_set_size_request(box[2], 150, 40);
    
    button[3] = gtk_button_new_with_label("   添加好友   ");
    g_signal_connect(G_OBJECT(button[3]), "clicked", G_CALLBACK(gtk_main_quit), "3");
    //g_signal_connect_swapped(G_OBJECT(button[3]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box[3]), button[3], FALSE, FALSE, 5);
    //sep = gtk_hseparator_new();
    //gtk_box_pack_start(GTK_BOX(box[3]), sep, FALSE, FALSE, 10);
    //gtk_widget_set_size_request(box[3], 150, 40);
    
    button[4] = gtk_button_new_with_label("   删除好友   ");
    g_signal_connect(G_OBJECT(button[4]), "clicked", G_CALLBACK(gtk_main_quit), "4");
    //g_signal_connect_swapped(G_OBJECT(button[4]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box[4]), button[4], FALSE, FALSE, 5);
    //sep = gtk_hseparator_new();
    //gtk_box_pack_start(GTK_BOX(box[4]), sep, FALSE, FALSE, 10);
    //gtk_widget_set_size_request(box[4], 150, 40);

    
    button[5] = gtk_button_new_with_label("   我的群聊   ");
    g_signal_connect(G_OBJECT(button[5]), "clicked", G_CALLBACK(gtk_main_quit), "5");
    //g_signal_connect_swapped(G_OBJECT(button[5]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box[5]), button[5], FALSE, FALSE, 5);
    //sep = gtk_hseparator_new();
    //gtk_box_pack_start(GTK_BOX(box[5]), sep, FALSE, FALSE, 10);
    //gtk_widget_set_size_request(box[5], 150, 40);

    button[6] = gtk_button_new_with_label("   新建群聊   ");
    g_signal_connect(G_OBJECT(button[6]), "clicked", G_CALLBACK(gtk_main_quit), "6");
    //g_signal_connect_swapped(G_OBJECT(button[6]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box[6]), button[6], FALSE, FALSE, 5);
    //sep = gtk_hseparator_new();
    //gtk_box_pack_start(GTK_BOX(box[6]), sep, FALSE, FALSE, 10);
    //gtk_widget_set_size_request(box[6], 150, 40);
    
    button[7] = gtk_button_new_with_label("      退出      ");
    g_signal_connect(G_OBJECT(button[7]), "clicked", G_CALLBACK(gtk_main_quit), "6");
    //g_signal_connect_swapped(G_OBJECT(button[7]), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box[7]), button[7], FALSE, FALSE, 5);
    //sep = gtk_hseparator_new();
    //gtk_box_pack_start(GTK_BOX(box[7]), sep, FALSE, FALSE, 10);
    //gtk_widget_set_size_request(box[7], 150, 40);

    image = gtk_image_new_from_file("main.png");
    gtk_box_pack_start(GTK_BOX(bigBox), image, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();    
}

void registeredFunction(GtkWidget *widget, gpointer *entry[], int argc, char *argv[])
{
    ALL regAll;
    
    printf("number:%d\n", number);

    regAll.reginfo.userinfo.usernum = number;
    printf("usernum赋值完成, 值为:%d\n", regAll.reginfo.userinfo.usernum);
    strcpy(regAll.reginfo.userinfo.username, gtk_entry_get_text(GTK_ENTRY(entry[0])));
    printf("username赋值完成, 值为:%s\n", regAll.reginfo.userinfo.username);
    strcpy(regAll.reginfo.password, gtk_entry_get_text(GTK_ENTRY(entry[1])));
    printf("password赋值完成, 值为:%s\n", regAll.reginfo.password);
    regAll.reginfo.userinfo.age = atoi(gtk_entry_get_text(GTK_ENTRY(entry[2])));
    printf("age赋值完成, 值为:%d\n", regAll.reginfo.userinfo.age);
    strcpy(regAll.reginfo.userinfo.sentence, gtk_entry_get_text(GTK_ENTRY(entry[3])));
    printf("sentence赋值完成,值为:%s\n", regAll.reginfo.userinfo.sentence);
    strcpy(regAll.reginfo.userinfo.sex, gtk_entry_get_text(GTK_ENTRY(entry[4])));
    printf("sex赋值完成, 值为:%s\n", regAll.reginfo.userinfo.sex);
    regAll.reginfo.userinfo.date.month = atoi(gtk_entry_get_text(GTK_ENTRY(entry[5])));
    printf("month赋值完成, 值为:%d\n", regAll.reginfo.userinfo.date.month);
    regAll.reginfo.userinfo.date.day = atoi(gtk_entry_get_text(GTK_ENTRY(entry[6])));
    printf("day赋值完成, 值为:%d\n", regAll.reginfo.userinfo.date.day);
    strcpy(regAll.reginfo.userinfo.constell, gtk_entry_get_text(GTK_ENTRY(entry[7])));
    printf("constell赋值完成, 值为:%s\n", regAll.reginfo.userinfo.constell);
    
    printf("赋值完成之后\n");

    printf("number :%d\n", regAll.reginfo.userinfo.usernum);
    printf("username :%s\n", regAll.reginfo.userinfo.username);
    printf("password :%s\n", regAll.reginfo.password);
    printf("age :%d\n", regAll.reginfo.userinfo.age);
    printf("sentence : %s\n", regAll.reginfo.userinfo.sentence);
    printf("sex :%s\n", regAll.reginfo.userinfo.sex);
    printf("month :%d\n", regAll.reginfo.userinfo.date.month);
    printf("day :%d\n", regAll.reginfo.userinfo.date.day);
    printf("constell :%s\n", regAll.reginfo.userinfo.constell);

    
    //现在把注册信息发送给服务器
    if(send(conn_fd, &regAll, sizeof(ALL), 0) < 0)
    {
        my_error("send", __LINE__);
    }

    //接下来从套接字上读取一次数据, 判断注册是否成功
    //一般来说肯定注册成功
    if(recv(conn_fd, &getResult, sizeof(getResult), 0) < 0)
    {
        my_error("recv", __LINE__);
    }

    getResult = FAIL;
}

void registeredInterface(GtkWidget *widget, gpointer data, int argc, char *argv[])
{
	GtkWidget *window;              //窗口
    GtkWidget *bigBox;              //含有用户名盒子及密码盒子的大盒子
    GtkWidget *box[8];              //用户名标签及编辑框盒子
    GtkWidget *sigBox;
    GtkWidget *label[8];
    GtkWidget *toplabel;            //分配帐号的结果 
    GtkWidget *monthlable;
    GtkWidget *daylable;
    GtkWidget *button;              //确定按钮
    GtkWidget *sep;                 //设置间距
    GtkWidget *image;               //登录界面的图片
    GList *sexGlist;                 //性别列表
    GtkWidget *sexCombo;            //性别组合框
    GList *ageGlist;                 //年龄列表
    GtkWidget *ageCombo;            //年龄组合框
    GList *monthGlist;               //月份列表
    GtkWidget *monthCombo;          //月份组合框
    GList *dayGlist;                 //日列表
    GtkWidget *dayCombo;            //日组合框
    GList *constellGlist;            //星座列表
    GtkWidget *constellCombo;       //星座组合框
    static GtkWidget *entry[8];     //编辑框
    char *month[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
    char *day[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
    
    char *constell[] = {"白羊座", "金牛座", "双子座", "巨蟹座", "狮子座", "处女座", "天秤座", "天蝎座", "射手座", "摩羯座", "水瓶座", "双鱼座"};
    int i = 0;
    int j = 0;
    char usernum[9] = {0};
    char tmpstring[25];
    char oneMark;

    oneMark = REG;
    if(send(conn_fd, &oneMark, sizeof(oneMark), 0) < 0)     //第一次send发送成功, 发送第一层标志位
    {
        my_error("send", __LINE__);
    }
    else
    {
        printf("标志位发送成功\n");
        //现在要从套接字上读取系统分配的帐号
        if(recv(conn_fd, &number, sizeof(number), 0) < 0)
        {
            my_error("recv", __LINE__);
        }
    }

    itoa(number, usernum, 10);
    strcpy(tmpstring, "您的帐号为:");
    strcat(tmpstring, usernum);
   
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "注册窗口");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    bigBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), bigBox);

    image = gtk_image_new_from_file("reg.png");
    gtk_box_pack_start(GTK_BOX(bigBox), image, FALSE, FALSE, 0);

    toplabel = gtk_label_new(tmpstring);
    gtk_box_pack_start(GTK_BOX(bigBox), toplabel, FALSE, FALSE, 0);
    gtk_widget_set_size_request(toplabel, 200, 50);
    for(j = 0; j < 8; j++)
    {
        box[j] = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(bigBox), box[j], FALSE, FALSE, 0);
    }

    sep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(bigBox), sep, FALSE, FALSE, 5);

	label[i] = gtk_label_new("昵称");
    entry[0] = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box[i]), entry[0], FALSE, FALSE, 1);
    gtk_entry_set_max_length((GtkEntry *)entry[0], 8);      //昵称最多只能有8个字
    gtk_container_set_border_width(GTK_CONTAINER(box[i]), 5);
	gtk_widget_set_size_request(label[i], 50, 33);
    gtk_widget_set_size_request(entry[0], 150, 33);
	gtk_widget_set_size_request(box[i], 350, 40);
    i++;

    label[i] = gtk_label_new("密码");
    entry[1] = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry[1]), FALSE);       //第二个参数为false可保证输入的密码显示为*
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box[i]), entry[1], FALSE, FALSE, 1);
    gtk_entry_set_max_length((GtkEntry *)entry[1], 16);         //密码最多有16位
	gtk_widget_set_size_request(label[i], 50, 33);
    gtk_widget_set_size_request(entry[1], 150, 33);
	gtk_widget_set_size_request(box[i], 350, 40);
    i++;

	label[i] = gtk_label_new("性别");
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    sexGlist = NULL;
    sexGlist = g_list_append(sexGlist, "男");
    sexGlist = g_list_append(sexGlist, "女");
    sexCombo = gtk_combo_new();
    gtk_combo_set_popdown_strings(GTK_COMBO(sexCombo), sexGlist);
    gtk_box_pack_start(GTK_BOX(box[i]), sexCombo, TRUE, TRUE, 1);
	gtk_widget_set_size_request(label[i], 60, 33);
    gtk_widget_set_size_request(sexCombo, 10, 33);
	gtk_widget_set_size_request(box[i], 350, 40);

	label[i] = gtk_label_new("年龄");
    entry[2] = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box[i]), entry[2], FALSE, FALSE, 1);
	gtk_widget_set_size_request(label[i], 60, 33);
    gtk_widget_set_size_request(entry[2], 50, 33);
	gtk_widget_set_size_request(box[i], 350, 40);
    i++;

	label[i] = gtk_label_new("生日");
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    monthGlist = NULL;
    for(j = 0; j < 12; j++)
    {
        monthGlist = g_list_append(monthGlist, month[j]);
    }
    monthCombo = gtk_combo_new();
    gtk_combo_set_popdown_strings(GTK_COMBO(monthCombo), monthGlist);
    gtk_box_pack_start(GTK_BOX(box[i]), monthCombo, TRUE, TRUE, 5);
	gtk_widget_set_size_request(label[i], 60, 33);
    gtk_widget_set_size_request(monthCombo, 20, 33);

    monthlable = gtk_label_new("月");
    gtk_box_pack_start(GTK_BOX(box[i]), monthlable, FALSE, FALSE, 5);
	gtk_widget_set_size_request(monthlable, 20, 33);

    dayGlist = NULL;
    for(j = 0; j < 31; j++)
    {
        dayGlist = g_list_append(dayGlist, day[j]);
    }
    dayCombo = gtk_combo_new();
    gtk_combo_set_popdown_strings(GTK_COMBO(dayCombo), dayGlist);
    gtk_box_pack_start(GTK_BOX(box[i]), dayCombo, TRUE, TRUE, 5);
    gtk_widget_set_size_request(dayCombo, 20, 33);

    daylable = gtk_label_new("日");
    gtk_box_pack_start(GTK_BOX(box[i]), daylable, FALSE, FALSE, 0);
    gtk_widget_set_size_request(daylable, 20, 33);
	gtk_widget_set_size_request(box[i], 350, 40);
    i++;

	label[i] = gtk_label_new("星座");
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);

    constellGlist = NULL;
    for(j = 0; j < 13; j++)
    {
        constellGlist = g_list_append(constellGlist, constell[j]);
    }
    constellCombo = gtk_combo_new();
    gtk_combo_set_popdown_strings(GTK_COMBO(constellCombo), constellGlist);
    gtk_box_pack_start(GTK_BOX(box[i]), constellCombo, TRUE, TRUE, 5);
	gtk_widget_set_size_request(label[i], 60, 33);
    gtk_widget_set_size_request(constellCombo, 20, 33);
	gtk_widget_set_size_request(box[i], 350, 40);
    i++;

	label[i] = gtk_label_new("个性签名");
    entry[3] = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box[i]), entry[3], FALSE, FALSE, 1);
    gtk_entry_set_max_length((GtkEntry *)entry[3], 15);
	gtk_widget_set_size_request(label[i], 60, 33);
    gtk_widget_set_size_request(entry[3], 200, 33);
	gtk_widget_set_size_request(box[i], 350, 40);
    i++;
    
    entry[4] = GTK_COMBO(sexCombo)->entry;
    entry[5] = GTK_COMBO(monthCombo)->entry;
    entry[6] = GTK_COMBO(dayCombo)->entry;
    entry[7] = GTK_COMBO(constellCombo)->entry;

    int k;

    for(k = 0; k < 8; k++)
    {
        printf("entry[%d]:%x->%s\n", k, GTK_ENTRY(entry[k]), gtk_entry_get_text(GTK_ENTRY(entry[k])));
    }

	button = gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(box[i]), button, FALSE, FALSE, 5);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(registeredFunction), entry);//此处回调注册函数
    g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_box_pack_start(GTK_BOX(bigBox), button, FALSE, FALSE, 5);
	gtk_widget_set_size_request(button, 50, 30);   

    gtk_widget_show_all(window);
    gtk_main();
}

void messagePromptInterface(int argc, char *argv[])
{
    GtkWidget *button;
    GtkWidget *frame;       //对话框
    GtkWidget *label;       //对话框里面的标签
    GtkWidget *yesButton;   //yes按钮
    GtkWidget *noButton;    //no按钮
    GtkWidget *box;
    GtkWidget *box1;
    GtkWidget *image;

    gtk_init(&argc, &argv);
    messageWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(messageWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_window_set_title(GTK_WINDOW(messageWindow), "登录失败");
    gtk_container_set_border_width(GTK_CONTAINER(messageWindow), 20);

    box = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(messageWindow), box);

    image = gtk_image_new_from_file("loginFail.png");
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 3);

    label = gtk_label_new("帐号不存在或密码错误");
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 5);

    box1 = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), box1, FALSE, FALSE, 8);

    yesButton = gtk_button_new_with_label("重新登录");
    g_signal_connect(G_OBJECT(yesButton), "clicked", G_CALLBACK(client_main), NULL);
    gtk_box_pack_start(GTK_BOX(box1), yesButton, FALSE, FALSE, 5);

    noButton = gtk_button_new_with_label("    取消    ");
    g_signal_connect(G_OBJECT(noButton), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box1), noButton, FALSE, FALSE, 5);
    
    gtk_widget_show_all(messageWindow);
    gtk_main();
}

void loginFunction(GtkWidget *widget, gpointer data)
{
    ALL all;                //储存要发送给服务器的登录信息结构体
    char state;             //得到在线还是隐身的状态
    const gchar *usernum;   //得到输入的帐号
    const gchar *password;  //得到输入的密码
    char oneMark;           //第一层标志位
    char recvBuf[BUFSIZE] = {0};
    //char getResult;
    
    logFail = SUCCESS;

    usernum = gtk_entry_get_text(GTK_ENTRY(entry1));
    password = gtk_entry_get_text(GTK_ENTRY(entry2));
    state = GPOINTER_TO_INT(data);
    //printf("%c\n", state);
    
    all.loginfo.usernum = atoi(usernum);
    strcpy(all.loginfo.password, password);
    all.loginfo.state = state;
    
    //printf("%c\n", loginfo.state);

    g_print("帐号是:%d\n", all.loginfo.usernum);
    g_print("密码是:%s\n", all.loginfo.password);
    g_print("状态时:%c\n", all.loginfo.state);

    oneMark = LOG;
    if(send(conn_fd, &oneMark, sizeof(oneMark), 0) < 0)      //第一次send要发送了!!!!!!!
    {
        my_error("send", __LINE__);
    }
    else
    {
        printf("标志位发送成功!\n");
        if(send(conn_fd, &all, sizeof(ALL), 0) < 0)  //第二次send要发送了!!!!!!!
        {
            my_error("send", __LINE__);
        }
        //接下来从套接字上读取一次数据, 判断登录是否成功
        //如果成功将进行下一步, 显示下一个窗口
        //如果不成功, 显示一个框口
        //提示帐号不存在或密码错误, 然后显示一个消息框, 两个按钮, 一个是重新登录, 另一个是取消

        if(recv(conn_fd, &getResult, sizeof(getResult), 0) < 0)     //第一次recv了!!!!!!
        {
            printf("data is too long\n");
            exit(1);
        }
        printf("getResult为:%c\n", getResult);
    }
}

char aboutState(GtkWidget *widget, gpointer data)
{
    //按钮被选择时的代码
    
    if(GTK_TOGGLE_BUTTON(widget)->active)
    {
        g_print("隐身\n");
        
        return STEALTH;
    }
    else
    {
        g_print("在线\n");
        
        return ONLINE;
    }
}

void loginInterface(int argc, char *argv[])
{
  	GtkWidget *window;      //窗口
    GtkWidget *box;         //含有用户名盒子及密码盒子的大盒子
    GtkWidget *box1;        //用户名标签及编辑框盒子
    GtkWidget *box2;        //密码标签及编辑框盒子
    GtkWidget *box3;        //在线隐身选项盒子
    GSList *group;          //生成radio按钮的组
    GtkWidget *radio;       //在线和隐身按钮
    GtkWidget *label1;      //登录标签
    GtkWidget *label2;      //注册标签
    GtkWidget *button1;     //登录按钮
    GtkWidget *button2;     //注册按钮
    GtkWidget *sep;         //设置间距
    GtkWidget *image;       //登录界面的图片
    char mark;              //接收判断在线或隐身后的标志

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
    //printf("%s\n", tmpstring);

	//以下生成两个radio按钮, 将他们加入到box容器中, 并显示出来
    box3 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), box3, FALSE, FALSE, 5);
    radio = gtk_radio_button_new_with_label(NULL, "在线");
    g_signal_connect(G_OBJECT(radio), "clicked", G_CALLBACK(aboutState), NULL);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio), TRUE);
    gtk_box_pack_start(GTK_BOX(box3), radio, TRUE, TRUE, 50);

    group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio));
    radio = gtk_radio_button_new_with_label(group, "隐身");
    g_signal_connect(G_OBJECT(radio), "clicked", G_CALLBACK(aboutState), NULL);
    gtk_box_pack_start(GTK_BOX(box3), radio, TRUE, TRUE, 50);
    
    mark = aboutState(radio, NULL);
    
	button1 = gtk_button_new_with_label("登录");
    g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(loginFunction), &mark);//此处回调登录函数
    g_signal_connect_swapped(G_OBJECT(button1), "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE, 5);
    
    button2 = gtk_button_new_with_label("注册");
    g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(registeredInterface), NULL);//此处回调注册函数
    //g_signal_connect_swapped(G_OBJECT(button2), "clicked", G_CALLBACK(gtk_main_quit), window);
    gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 5);

    image = gtk_image_new_from_file("login.png");
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 3);
   
    //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(changedResult), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();  
}

void client_init(int argc,char *argv[])
{
    struct sockaddr_in serv_addr;       //服务器地址结构
    int i;
    
    //检查参数个数, 参数个数应为3, 不是3, 则直接退出
    if(argc != 3)
    {
        printf("Usage:[-a] [serv_address]\n");
        exit(1);
    }

    //初始化服务器地址结构
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;             //初始化地址类型
    serv_addr.sin_port = htons(4507);           //初始化端口号
    
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

    printf("%d\n", conn_fd);

    //向服务器端发送连接请求

    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
    {
        my_error("connect", __LINE__);
    }
    
    printf("%d\n", conn_fd);

}

void client_main(int argc, char *argv[])
{
    pthread_t recvPid;
    ALL userinfoAll;

    client_init(argc, argv); 
    
    //先要提前处理一下登录失败时弹出的消息框
    gtk_widget_destroy(messageWindow);

    //调用登录界面函数
    loginInterface(argc, argv);

    //如果登录失败又取消重新登录, 那么就结束这个客户端的进程
    if(logFail == FAIL)
    {
        exit(0);
    }
    
    //如果登录失败, 调用对话框提示重新登录或者取消
    if(getResult == FAIL)
    {
        messagePromptInterface(argc, argv);
    }
    else
    {
        //登录成功, 先再服务器接收一个有用户信息的结构体, 为主界面的显示提供信息
        if(recv(conn_fd, &userinfoAll, sizeof(ALL), 0) < 0)
        {
            my_error("recv", __LINE__);
        }
        strcpy(username, userinfoAll.userinfo.username);

        //再创建一个recv线程, 然后调用主界面函数
        pthread_create(&recvPid, NULL, (void *)client_recv, NULL);
        
        mainInterface(argc, argv);
    }
}

void itoa(unsigned long val, char *buf, unsigned radix) 
{ 
    char *p;  
    char *firstdig; 
    char temp; 
    unsigned digval; 

    p = buf; 
    firstdig = p; 

    do { 
        digval = (unsigned) (val % radix); 
        val /= radix; 

        if (digval > 9) 
        *p++ = (char ) (digval - 10 + 'a'); 
        else 
        *p++ = (char ) (digval + '0');
    } while (val > 0); 


    *p-- = '\0'; 

    do { 
        temp = *p; 
        *p = *firstdig; 
        *firstdig = temp; 
        --p; 
        ++firstdig; 
    } while (firstdig < p); 
}

void my_error(const char *err_string, int line)
{
    fprintf(stderr, "line:%d ", line);
    perror(err_string);
    exit(1);
}

int main(int argc,char *argv[])
{
	client_main(argc, argv);
}
