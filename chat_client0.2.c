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

#define BUFSIZE         1024

#define FAIL            0
#define SUCCESS         1

#define ONLINE          'l'
#define STEALTH         's'

#define OFF             0
#define LOG             1
#define REG             2
#define MESSAGE         3

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
    DATE day;               //生日
    char constell[8];       //星座
    char sentence[40];      //个性签名, 不超过十五个字, 设置个性签名时应该检查字数
}USERINFO;

typedef struct REGINFO      //用户注册信息结构体
{
    char password[17];          //不超过16位的密码
    USERINFO userinfo;          //用户基本信息
    FRIENDINFO *friendlist;     //好友列表
}REGINFO;

typedef struct LOGINFO      //用户登录结构体
{
    int usernum;
    char password[17];
    char state;             //状态
}LOGINFO;

static GtkWidget *entry1;
static GtkWidget *entry2;
int number = 16081101;      //由系统统一分配的帐号
int conn_fd;

void my_error(const char *err_string, int line);
void my_gtk_main(int argc, char *argv[]);
void loginInterface(int argc, char *argv[]);
void loginFunction(int argc, char *argv[], GtkWidget *button, gpointer *data);
void client_main(int argc, char *argv[]);
char aboutState(GtkWidget *widget, gpointer data);
void registeredInterface();
void messagePromptInterface(int argc, char *argv[]);
void registeredInterface(int argc, char *argv[]);
void itoa(unsigned long val, char *buf, unsigned radix);

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

void registeredInterface(int argc, char *argv[])
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

    itoa(number, usernum, 10);
    strcpy(tmpstring, "您的帐号为:");
    strcat(tmpstring, usernum);
   
    number++;

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
    entry[i] = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box[i]), entry[i], FALSE, FALSE, 1);
    gtk_entry_set_max_length((GtkEntry *)entry[i], 8);      //昵称最多只能有8个字
    gtk_container_set_border_width(GTK_CONTAINER(box[i]), 5);
	gtk_widget_set_size_request(label[i], 50, 33);
    gtk_widget_set_size_request(entry[i], 150, 33);
	gtk_widget_set_size_request(box[i], 350, 40);
    i++;

    label[i] = gtk_label_new("密码");
    entry[i] = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry[i]), FALSE);       //第二个参数为false可保证输入的密码显示为*
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box[i]), entry[i], FALSE, FALSE, 1);
    gtk_entry_set_max_length((GtkEntry *)entry[i], 16);         //密码最多有16位
	gtk_widget_set_size_request(label[i], 50, 33);
    gtk_widget_set_size_request(entry[i], 150, 33);
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
    entry[i] = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box[i]), entry[i], FALSE, FALSE, 1);
	gtk_widget_set_size_request(label[i], 60, 33);
    gtk_widget_set_size_request(entry[i], 50, 33);
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
    for(j = 0; j < 12; j++)
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
    entry[i] = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box[i]), label[i], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box[i]), entry[i], FALSE, FALSE, 1);
    gtk_entry_set_max_length((GtkEntry *)entry[i], 15);
	gtk_widget_set_size_request(label[i], 60, 33);
    gtk_widget_set_size_request(entry[i], 200, 33);
	gtk_widget_set_size_request(box[i], 350, 40);
    i++;

	button = gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(box[i]), button, FALSE, FALSE, 
5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);//此处回调登录函数
    g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), window);
    gtk_box_pack_start(GTK_BOX(bigBox), button, FALSE, FALSE, 5);
	gtk_widget_set_size_request(button, 50, 30);   

    gtk_widget_show_all(window);
    gtk_main();
}

void messagePromptInterface(int argc, char *argv[])
{
	GtkWidget *window;
    GtkWidget *button;
    GtkWidget *frame;       //对话框
    GtkWidget *label;       //对话框里面的标签
    GtkWidget *yesButton;   //yes按钮
    GtkWidget *noButton;    //no按钮
    GtkWidget *box;
    GtkWidget *box1;
    GtkWidget *image;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_window_set_title(GTK_WINDOW(window), "登录失败");
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    box = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    image = gtk_image_new_from_file("loginFail.png");
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 3);

    label = gtk_label_new("帐号不存在或密码错误");
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 5);

    box1 = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), box1, FALSE, FALSE, 8);

    yesButton = gtk_button_new_with_label("重新登录");
    g_signal_connect(G_OBJECT(yesButton), "clicked", G_CALLBACK(loginInterface), NULL);
    gtk_box_pack_start(GTK_BOX(box1), yesButton, FALSE, FALSE, 5);

    noButton = gtk_button_new_with_label("    取消    ");
    g_signal_connect(G_OBJECT(noButton), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(box1), noButton, FALSE, FALSE, 5);
    
    gtk_widget_show_all(window);
    gtk_main();
}

char aboutState(GtkWidget *widget, gpointer data)
{
    //按钮被选择时的代码
    
    if(GTK_TOGGLE_BUTTON(widget)->active)
    {
        //g_print("在线\n");
        
        return ONLINE;
    }
    else
    {
        //g_print("隐身\n");
        
        return STEALTH;
    }
}

void client_main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;       //服务器地址结构
    //int conn_fd;                        //连接套接字
    int i;
    
    //检查参数个数, 参数个数应为3, 不是3, 则直接退出
    if(argc != 3)
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
    loginInterface(argc, argv);

}

void loginFunction(int argc, char *argv[], GtkWidget *button, gpointer *data)
{
    LOGINFO loginfo;        //储存要发送给服务器的登录信息结构体
    char state;             //得到在线还是隐身的状态
    const gchar *usernum;   //得到输入的帐号
    const gchar *password;  //得到输入的密码
    char oneMark;           //第一层标志位
    char recvBuf[BUFSIZE] = {0};
    int getResult;
    

    usernum = gtk_entry_get_text(GTK_ENTRY(entry1));
    password = gtk_entry_get_text(GTK_ENTRY(entry2));
    state = GPOINTER_TO_INT(*data);
    //printf("%d\n", state);
    //printf("%c\n", state);
    
    loginfo.usernum = atoi(usernum);
    strcpy(loginfo.password, password);
    loginfo.state = state;
    
    //printf("%c\n", loginfo.state);

    //g_print("帐号是:%d\n", loginfo.usernum);
    //g_print("密码是:%s\n", loginfo.password);
    //g_print("状态时:%c\n", loginfo.state);

    //这时要先发送一个标志位, 告诉服务器我要发送的数据结构体类型

    oneMark = LOG;
    if(send(conn_fd, &oneMark, sizeof(oneMark), 0) < 0)      //第一次send要发送了!!!!!!!
    {
        my_error("send", __LINE__);
    }
    else
    {
        printf("标志位发送成功!\n");
        if(send(conn_fd, &loginfo, sizeof(LOGINFO), 0) < 0)  //第二次send要发送了!!!!!!!
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
        
        //如果登录不成功, 显示一个消息提示框
        if(getResult == FAIL)
        {
            messagePromptInterface(argc, argv);
        }
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
    GtkWidget *lRadio;      //在线按钮
    GtkWidget *sRadio;      //隐身按钮
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

	//以下生成两个radio按钮, 将他们加入到box容器中, 并显示出来
    box3 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), box3, FALSE, FALSE, 5);
    lRadio = gtk_radio_button_new_with_label(NULL, "在线");
    //g_signal_connect(G_OBJECT(radio), "clicked", G_CALLBACK(aboutState), NULL);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lRadio), TRUE);
    gtk_box_pack_start(GTK_BOX(box3), lRadio, TRUE, TRUE, 50);

    group = gtk_radio_button_group(GTK_RADIO_BUTTON(lRadio));
    sRadio = gtk_radio_button_new_with_label(group, "隐身");
    //g_signal_connect(G_OBJECT(radio), "clicked", G_CALLBACK(aboutState), NULL);
    gtk_box_pack_start(GTK_BOX(box3), sRadio, TRUE, TRUE, 50);
    
    g_signal_connect(G_OBJECT(lRadio), "clicked", G_CALLBACK(aboutState), NULL);
    mark = aboutState(lRadio, NULL);


	button1 = gtk_button_new_with_label("登录");
    g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(loginFunction), &mark);//此处回调登录函数
    g_signal_connect_swapped(G_OBJECT(button1), "clicked", G_CALLBACK(gtk_main_quit), window);
    gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE, 5);
    
    button2 = gtk_button_new_with_label("注册");
    g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(registeredInterface), NULL);//此处回调注册函数
    g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(gtk_main_quit), window);
    gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 5);

    image = gtk_image_new_from_file("login.png");
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 3);
    g_signal_connect(G_OBJECT(lRadio), "clicked", G_CALLBACK(aboutState), NULL);
    //g_signal_connect(G_OBJECT(sRadio), "clicked", G_CALLBACK(aboutState), NULL);
    
    //mark = aboutState(lRadio, NULL);
    //printf("登录函数中的mark:%d\n", mark);
    
    gtk_widget_show_all(window);
    gtk_main();
}

void my_gtk_main(int argc, char *argv[])
{
    loginInterface(argc, argv); 
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
    //client_main(argc, argv);
}
