#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

int bavcount = 0;           //Bav记录
int hirercount = 0;         //Hirer记录
int bavnum = 10000001;      //Bav的序号是默认增加的
int hirernum = 10000001;    //hirer的序号也是默认增加的
//Bav信息

typedef struct BavInfo
{
    int bavnum;        //Bav的编号
    char bavname[30];  //Bav的名字
    char author[20];   //作者的名字
    char bavkind[10];     //属性
    float price;       //原价
    float bavprice;    //租借的价格
    int inventory;     //库存
    struct BavInfo *next;//链域
}BavInfo;

//年月日

typedef struct Date
{
    int year;          //年
    int month;         //月
    int day;           //日
}Date;

//租赁Bav信息

typedef struct HireBav
{
    BavInfo bavinfo;   //Bav信息
    Date startday;     //租借日
    Date enday;        //归还日
    int overday;       //过期天数
    float money;       //金额(价格+罚金)
    struct HireBav *next;
}HireBav;

//租赁者信息

typedef struct Hirer
{
    int hirernum;      //租借者编号
    char hirername[20];//租借者名字
    char hirerphonenum[12];//租借者电话号码
    struct HireBav hirebav[5];   //租借者租借的Bav数组
    int hireramount;             //租借的Bav本数
    struct Hirer *next;
}Hirer;


//功能函数



void *Malloc(size_t size);            //申请节点内存的函数
void saveBavInfo(BavInfo head);
void inputBavInfo(BavInfo *head);
BavInfo *isExisted(BavInfo head, char bavname[30], char author[20]);
int readBavInfo(BavInfo head);
void printBavInfo(BavInfo head);

void HirerSystem(Hirer *hirerhead, BavInfo *bavhead);          //租借系统
Hirer *isExistedHirer(Hirer head, int tmphirernum);        //按照名字和作者查询该Bav是否存在
Hirer *inputHirerInfo(Hirer *head);                       //输入HirerInfo的信息
int inputHirernum(Hirer *head);                         //输入帐号
void hirerSomeBav(Hirer *hirerhead, BavInfo *bavhead, Hirer *p);                         //租借Bav
void saveHirerInfo(Hirer head);
void printHirersInfo(Hirer head);
int readHirerInfo(Hirer head);
void returnSystem(Hirer *hirerhead, BavInfo *bavhead);
int returnHirernum(Hirer *head);
void returnSomeBav(Hirer *hirerhead, BavInfo *bavhead, Hirer *p);
BavInfo *isExistedByBavnum(BavInfo head, int tmpbavnum);
void printHirerInfo(Hirer people);
int isLeapyear(int year);
int calculateOverday(Date *startday, Date *enday);
void deleteBav(BavInfo *head);
void changePrice(BavInfo *head);
BavInfo *findBavInfoBynum(BavInfo head, int tmpbavnum);
void printOneBavInfo(BavInfo *info);
BavInfo *findBavInfoBynameandauthor(BavInfo head, char bavname[30], char author[20]);
Hirer *findHirerInfoBynum(Hirer head, int tmphirernum);
Hirer *findHirerInfoBynameandphone(Hirer head, char hirername[20], char hirerphonenum[12]);
void printOneHirerInfo(Hirer *p);
void menu(BavInfo *bavhead, Hirer *hirerhead);
void findBavInfo(BavInfo head);
void findHirerInfo(Hirer head);
void addBavInfo(BavInfo *head);
void havenumHirerSomeBav(Hirer *hirerhead, BavInfo *bavhead, Hirer *p);

void havenumHirerSomeBav(Hirer *hirerhead, BavInfo *bavhead, Hirer *p)
{
   // Hirer *q;
    BavInfo *r;
    int i;
    int endmark = 1;

    char tmpbavname[30];
    char tmpauthor[20];
//    struct HireBav hirebav[5];

	system("clear");
	printf("\t============================================================\n");
    printf("\t现在开始租借Bav!\n\n");
    printf("\t每个帐号最多可以租借5本Bav!\n");
	printf("\t============================================================\n");
    
    //p->hireramount = 0;

    for(i = p->hireramount; i < 5 && endmark; i++)
    {
        printf("\t请输入您要租借的Bav的名字和作者(如:小时代1 郭敬明):");
        scanf("%s", tmpbavname);
        scanf("%s", tmpauthor);

        if((r = isExisted(*bavhead, tmpbavname, tmpauthor)) != NULL)
        {
            p->hirebav[i].bavinfo.bavnum = r->bavnum;
            strcpy(p->hirebav[i].bavinfo.bavname, r->bavname);
            strcpy(p->hirebav[i].bavinfo.author, r->author);
            strcpy(p->hirebav[i].bavinfo.bavkind, r->bavkind);
            p->hirebav[i].bavinfo.price = r->price;
            p->hirebav[i].bavinfo.bavprice = r->bavprice;
            p->hirebav[i].bavinfo.inventory = --r->inventory;

            printf("\t请输入租借的日期(输入格式如:2016 1 1):");
            scanf("%d %d %d", &p->hirebav[i].startday.year, &p->hirebav[i].startday.month, &p->hirebav[i].startday.day);
            p->hirebav[i].enday.year = 0;               //这里只是租借,还没有归还,所以归还日期都为0;
            p->hirebav[i].enday.month = 0;              //过期天数也为0;
            p->hirebav[i].enday.day = 0;                //总钱数也只是租借十天的价格;
            p->hirebav[i].overday = 0;
            p->hirebav[i].money = r->bavprice;
           
			printf("\t============================================================\n\n");
            printf("\t第%d本Bav租借成功!\n", i+1);
            
            p->hireramount++;
        }
        else
        {
            printf("\t该Bav不存在或着库存为0!\n");
        }
		
		printf("\t============================================================\n");
        printf("\t继续租借请按1, 结束租借请按0：");
        scanf("%d", &endmark);
		printf("\t============================================================\n");
    }

    saveHirerInfo(*hirerhead);
}


void addBavInfo(BavInfo *head)
{
	BavInfo *p;
    BavInfo *q;
    BavInfo *r;         //接收isExisted返回过来的指针
    int mark = 1;       //添加结束标志
    int kind;

    char bavname[30];
    char author[20];
    char bavkind[10];
    float price;
    float bavprice;
    int inventory;

	for(q = head; q->next; q = q->next)
	{
		;
	}
	
	printf("\t============================================================\n");
    printf("\t现在开始添加Bav的信息:\n");
    printf("\t============================================================\n");

    while(mark)
    {
        printf("\t请输入Bav的名字:");
        scanf("%s", bavname);
        printf("\t请输入作者的名字:");
        scanf("%s", author);
        if(!isExisted(*head, bavname, author))
        {
            printf("\t请选择Bav的类型(1. 图书 2. audio 3.video):");
            scanf("%d", &kind);
            switch(kind)
            {
                case 1:strcpy(bavkind, "图书");
                break;
                
                case 2:strcpy(bavkind, "audio");
                break;
     
                case 3:strcpy(bavkind, "video");
                break;
            }
            printf("\t请输入该Bav的原价:");
            scanf("%f", &price);
            bavprice = price*0.05;
            inventory = 1;

            p = (BavInfo *)Malloc(sizeof(BavInfo));
            p->bavnum = bavnum;
            strcpy(p->bavname, bavname);
            strcpy(p->author, author);
            strcpy(p->bavkind, bavkind);
            p->price = price;
            p->bavprice = bavprice;
            p->inventory = inventory;
            p->next = NULL;

            q->next = p;

            bavnum++;
        }    
        else
        {
            //查询到这个bav的信息并给inventory + 1  
            r = isExisted(*head, bavname, author);
            r->inventory++;
        }
        bavcount++;
		printf("\t============================================================\n");
        printf("\t第%d个信息已经录入完毕,是否继续录入?\n", bavcount);
		printf("\t继续请按1，结束请按0:");
        scanf("%d", &mark);
        if(mark == 0)
        {
            saveBavInfo(*head);
        }
    }
}

void findHirerInfo(Hirer head)
{
    int n;
    int tmphirernum;
    char hirername[20];
    char hirerphonenum[30];
	Hirer *p;

    printf("\t请输入查找方式:\n\n");
    printf("\t============================================================\n");	
    printf("\t||-------- @ 1.按照用户姓名和手机号码查询 @ --------||\n\n");
    printf("\t||-------- @ 2.按用户帐号查询             @ --------||\n\n");
    printf("\t============================================================\n");		
    printf("\t请输入对应的数字以选择查询功能:");
    scanf("%d", &n);

    switch(n)
    {
        case 1:
        printf("\t请输入要查询的用户姓名和手机号码::");
        scanf("%s %s", hirername, hirerphonenum);
        p = findHirerInfoBynameandphone(head, hirername, hirerphonenum);
		printOneHirerInfo(p);
		break;

        case 2:
        printf("\t请输入用户的帐号:");
        scanf("%d", &tmphirernum);
        p = findHirerInfoBynum(head, tmphirernum);
		printOneHirerInfo(p);
		break;
    }
}


void findBavInfo(BavInfo head)
{
    int n;
    int tmpbavnum;
    char bavname[30];
    char author[20];
	BavInfo *p;

	printf("\t============================================================\n");
    printf("\t请输入查找方式:\n\n");
    printf("\t============================================================\n");	
    printf("\t||-------- @ 1.按Bav名字和作者查询 @ --------||\n\n");
    printf("\t||-------- @ 2.按Bav编号查询       @ --------||\n\n");
    printf("\t============================================================\n");		
    printf("\t请输入对应的数字以选择查询功能:");
    scanf("%d", &n);
	printf("\t============================================================\n");

    switch(n)
    {
        case 1:
        printf("\t请输入要查询的Bav的名字和作者名字:");
        scanf("%s %s", bavname, author);
        p = findBavInfoBynameandauthor(head, bavname, author);
		printOneBavInfo(p);	
		break;

        case 2:
        printf("\t请输入Bav的编号:");
        scanf("%d", &tmpbavnum);
        p = findBavInfoBynum(head, tmpbavnum);
		printOneBavInfo(p);
		break;
    }
}

void menu(BavInfo *bavhead, Hirer *hirerhead)
{
	int n = 0;
    int returnmark = 0;

	do
	{
		printf("\n\n\n\n\n");
		printf("\t\t*---您好，欢迎进入音像图书（Bav）租赁管理系统！---*\n");
		printf("\t============================================================\n");
		printf("\t||                                                        ||\n");
		printf("\t||-------- @ 1. 创建Bav库      2. 浏览Bav库信息 @ --------||\n");
		printf("\t||                                                        ||\n");
		printf("\t||-------- @ 3. 购进Bav        4. 修改Bav价格   @ --------||\n");
		printf("\t||                                                        ||\n");
		printf("\t||-------- @ 5. 报废Bav        6. 查找Bav信息   @ --------||\n");
		printf("\t||                                                        ||\n");
		printf("\t||-------- @ 7. 进入租借系统   8. 进入归还系统  @ --------||\n");
		printf("\t||                                                        ||\n");
		printf("\t||-------- @ 9. 查询用户信息   0. 退出管理系统  @ --------||\n");
		printf("\t||                                                        ||\n");
		printf("\t============================================================\n");
        printf("\t请在0-9中选择,以回车结束:");
        scanf("%d", &n);
      
        system("clear");

        switch(n)
        {
            case 1:  
            inputBavInfo(bavhead);
			printf("\t============================================================\n");
            printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
                system("clear");
            }
           	returnmark = 0;
			break;

            case 2: 
            printBavInfo(*bavhead);
			printf("\t============================================================\n");
            printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
				system("clear");
            }
            returnmark = 0;
			break;

            case 3:
            addBavInfo(bavhead);
			system("clear");
			printf("\t============================================================\n");
            printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
                system("clear");
            }
            returnmark = 0;
			break;

            case 4:
            changePrice(bavhead);
			printf("\t============================================================\n");
			printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
                system("clear");
            }
            returnmark = 0;
			break;

            case 5:
            deleteBav(bavhead);
			printf("\t============================================================\n");
			printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
                system("clear");
            }
            returnmark = 0;
			break;
            
            case 6:
            findBavInfo(*bavhead);
			printf("\t============================================================\n");
			printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
                system("clear");
            }
            returnmark = 0;
			break;

            case 7:
            HirerSystem(hirerhead, bavhead);
			printf("\t============================================================\n");
			printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
                system("clear");
            }
            returnmark = 0;
			break;

            case 8:
            returnSystem(hirerhead, bavhead);
			printf("\t============================================================\n");
			printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
                system("clear");
            }
            returnmark = 0;
			break;
            
            case 9:
            findHirerInfo(*hirerhead);
			printf("\t============================================================\n");
			printf("\t返回主菜单请按1:");
            scanf("%d", &returnmark);
			printf("\t============================================================\n");
            if(returnmark)
            {
                system("clear");
            }
            returnmark = 0;
			break;
			
			case 0:
			break;

        }
    }while(n != 0);
}

void printOneHirerInfo(Hirer *p)
{
	int i;

	if(p)
	{
		printf("\n\t姓名\t帐号\t电话号码\n");
        printf("\n\t%s\t%d\t%s\n", p->hirername, p->hirernum, p->hirerphonenum);
        
        if(p->hireramount == 0)
        {
            printf("\t您还未租借Bav!\n");
        }
        else
        {
			printf("\t您所租借的Bav详细信息如下:\n");
            for(i = 0; i < p->hireramount; i++)
			{
                printf("\n\t编号\t名字\t作者\t属性\t原价\t租价\t库存\n");
                printf("\n\t%d\t%s\t%s\t%s\t%.1f\t%.1f\t%d", p->hirebav[i].bavinfo.bavnum, 
                    p->hirebav[i].bavinfo.bavname, p->hirebav[i].bavinfo.author, 
                    p->hirebav[i].bavinfo.bavkind, p->hirebav[i].bavinfo.price, 
                    p->hirebav[i].bavinfo.bavprice, p->hirebav[i].bavinfo.inventory);

                printf("\n\n\t租借的日期\t归还的日期\t过期天数\t金额\n");
                printf("\n\t%d %d %d\t%d %d %d\t%d\t%.1f\n", p->hirebav[i].startday.year, 
                    p->hirebav[i].startday.month, p->hirebav[i].startday.day, 
                    p->hirebav[i].enday.year, p->hirebav[i].enday.month, 
                    p->hirebav[i].enday.day, p->hirebav[i].overday, p->hirebav[i].money);
            }
            printf("\t共计租借本数:");
            printf("%d\n", p->hireramount);
		}
	}
	else
	{
		printf("\t输入信息有误，查询失败！\n");
	}
}

Hirer *findHirerInfoBynameandphone(Hirer head, char hirername[20], char hirerphonenum[12])
{
	Hirer *p;

	for(p = head.next; p; p = p->next)
	{
		if(!strcmp(p->hirername, hirername) && !strcmp(p->hirerphonenum, hirerphonenum))
		{
			return p;
		}
	}
	return NULL;
}


Hirer *findHirerInfoBynum(Hirer head, int tmphirernum)
{
	Hirer *p;
	for(p = head.next; p; p = p->next)
	{
		if(tmphirernum == p->hirernum)
		{
			return p;
		}
	}
	return NULL;
}


BavInfo *findBavInfoBynameandauthor(BavInfo head, char bavname[30], char author[20])
{
	BavInfo *p;

	for(p = head.next; p; p = p->next)
	{
		if(!strcmp(p->bavname, bavname) && !strcmp(p->author, author))
        {
			return p;
		}
	}
	return NULL;
}

void printOneBavInfo(BavInfo *info)
{
	if(info != NULL)
	{
		printf("\n\t编号\t名字\t作者\t属性\t原价\t租价\t库存\n");
		printf("\n\t%d\t%s\t%s\t%s\t%.1f\t%.1f\t%d\n", info->bavnum, info->bavname, info->author, info->bavkind, info->price, info->bavprice, info->inventory);
	}
	else
	{
		printf("\t输入信息有误，查询失败！\n");
	}
}

BavInfo *findBavInfoBynum(BavInfo head, int tmpbavnum)
{
	BavInfo *p;

	for(p = head.next; p; p = p->next)
	{
		if(tmpbavnum == p->bavnum)
		{
			return p;
		}
	}

	return NULL;
}


void changePrice(BavInfo *head)
{
	char bavname[30];
	char author[20];
	BavInfo *r;

	printf("\t============================================================\n");
	printf("\t请输入要修改价格的Bav名字和作者:");
	scanf("%s %s", bavname, author);
	printf("\t============================================================\n");

	r = isExisted(*head, bavname, author);

	if(r == NULL)
	{
		printf("\t============================================================\n");
		printf("\t该Bav不存在，无法更改价格！\n");
		printf("\t============================================================\n");
		return;
	}
	printf("\t当前价格为：%.1f\n", r->price);
	printf("\t请输入新的价格：");
	scanf("%f", &r->price);
	r->bavprice = r->price * 0.05;

	printf("\t============================================================\n");
	printf("\t修改成功！\n");
	printf("\t============================================================\n");

	saveBavInfo(*head);
}

void deleteBav(BavInfo *head)
{	
	int tree = 0;
    BavInfo *p;
    BavInfo *q;
    
	q = head;
    p = head->next;

    while(p)
    {
        if(p->inventory == 0)
        {
            q->next = p->next;
            free(p);
            p = q->next;
			tree = 1;
        }
        else
        {
            q = p;
            p = p->next;
        }
    }

	if(tree)
	{
		printf("\t已将库存为0的Bav报废完毕！\n");
	}
	else
	{
		printf("\t没有库存为0的Bav，不需要进行报废处理！\n");
	}
   
	saveBavInfo(*head);
}

int calculateOverday(Date *startday, Date *enday)
{
    int i;
    int overday = 0;

    const int month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(startday->year == enday->year)
    {
        if(startday->month == enday->month)
        {
            overday = enday->day - startday->day;
        }
        else
        {
            for(i = startday->month+1; i < enday->month; i++)
            {
                overday += month[i];
            }
            
            overday += month[startday->month] - startday->day + enday->day;

            if(isLeapyear(startday->year))
            {
                if(startday->month <= 2 && enday->month > 2)
                {
                    overday++;
                }
            }
        }
    }
    else
    {
        for(i = startday->year+1; i < enday->year; i++)
        {
            if(isLeapyear(i))
            {
                overday += 366;
            }
            else
            {
                overday += 365;
            }
        }
        
        for(i = startday->month+1; i <= 12; i++)        //求startday距离年末的时间
        {
            overday += month[i];
        }
        overday += month[startday->month] - enday->day;

        if(startday->month <= 2)
        {
            if(isLeapyear(startday->year))
            {
                overday++;
            }
        }

        for(i = 1; i < enday->month; i++)
        {
            overday += month[i];
        }
        overday += enday->year;

        if(enday->month > 2)
        {
            if(isLeapyear(enday->year))
            {
                overday++;
            }
        }
    }
return overday;
} 


int isLeapyear(int year)
{
    if(year%4 == 0 && year%100 != 0 || year%400 == 0)   //闰年的判断方法:四年一闰,百年不闰,四百年再一闰
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void printHirerInfo(Hirer people)
{
    int i;
    Hirer *p;
    p = &people;

    printf("\n\t姓名\t帐号\t电话号码\n");
    printf("\n\t%s\t%d\t%s\n", p->hirername, p->hirernum, p->hirerphonenum);
            
    if(p->hireramount == 0)
    {
        printf("\t您还未租借Bav!\n");
    }
    else
    {
		printf("\t您所租借的Bav详细信息如下:\n");
        for(i = 0; i < p->hireramount; i++)
        {
            
            printf("\n\t编号\t名字\t作者\t属性\t原价\t租价\t库存\n");
            printf("\n\t%d\t%s\t%s\t%s\t%.1f\t%.1f\t%d", p->hirebav[i].bavinfo.bavnum, 
                p->hirebav[i].bavinfo.bavname, p->hirebav[i].bavinfo.author, 
                p->hirebav[i].bavinfo.bavkind, p->hirebav[i].bavinfo.price, 
                p->hirebav[i].bavinfo.bavprice, p->hirebav[i].bavinfo.inventory);

            printf("\n\t租借的日期\t归还的日期\t过期天数\t金额\n");
            printf("\n\t%d %d %d\t%d %d %d\t%d\t%.1f\n", p->hirebav[i].startday.year, 
                p->hirebav[i].startday.month, p->hirebav[i].startday.day, 
                p->hirebav[i].enday.year, p->hirebav[i].enday.month, 
                p->hirebav[i].enday.day, p->hirebav[i].overday, p->hirebav[i].money);
        }
        printf("\t共计租借本数:");
        printf("%d\n", p->hireramount);
    }	
}
BavInfo *isExistedByBavnum(BavInfo head, int tmpbavnum)
{
    BavInfo *p;

    for(p = head.next; p; p = p->next)
    {
        if(tmpbavnum == p->bavnum)
        {
            return p;
        }
    }
    return NULL;
}


void returnSomeBav(Hirer *hirerhead, BavInfo *bavhead, Hirer *p)
{
    int tmpbavnum;
    int i;
    int j;
    int index;
    int tree = 0;
    Date startday;
    Date enday;   
    BavInfo *r;

    printf("\t您当前的租借状况如下:\n");
    printHirerInfo(*p);
    printf("\n\t============================================================\n");
    printf("\t请输入您要还的Bav的编号:");
    scanf("%d", &tmpbavnum);
    r = isExistedByBavnum(*bavhead, tmpbavnum);

    for(i = 0; i < p->hireramount; i++)
    {
        if(p->hirebav[i].bavinfo.bavnum == tmpbavnum)
        {
            tree = 1;
			index = i;
        }
    }

    printf("\t请输入归还的时间(输入格式为:2016 1 1):");
    scanf("%d %d %d", &p->hirebav[index].enday.year, &p->hirebav[index].enday.month, &p->hirebav[index].enday.day);
    
    startday = p->hirebav[index].startday;
    enday = p->hirebav[index].enday;
    
    p->hirebav[index].overday = calculateOverday(&startday, &enday) - 10;

    if(p->hirebav[index].overday < 0 || p->hirebav[index].overday == 0)
    {
        p->hirebav[index].money = p->hirebav[index].bavinfo.bavprice;
    }
    else
    {
        p->hirebav[index].money = p->hirebav[index].bavinfo.bavprice + 
                    p->hirebav[index].overday * p->hirebav[index].bavinfo.bavprice * 0.5;
    }

//    p->hirebav[index].bavinfo.inventory++;

    printf("\t您需要付的租价为:%.1f\n", p->hirebav[index].money);

    if(tree == 0)
    {
		printf("\t============================================================\n");
        printf("\t您没有租借这本Bav!\n");
		printf("\t============================================================\n");
    }
    else
    {
       
        if(index != p->hireramount-1)
        {
            for(j = index; j < p->hireramount; j++)
            {
                p->hirebav[j] = p->hirebav[j+1];
            }
        }

        p->hirebav[index].bavinfo.inventory += 1;
        r->inventory += 1;
		printf("\t============================================================\n");
		printf("\t归还成功!\n");
		printf("\t============================================================\n");
 		
		p->hireramount--;
        
    }
  
    saveHirerInfo(*hirerhead);
    saveBavInfo(*bavhead);
	printf("\t您当前的租借状况如下:\n");
    printHirerInfo(*p);
}


int returnHirernum(Hirer *head)
{
    int tmphirernum;
    int renummark;
	
	printf("\t============================================================\n");
    printf("\t请输入您的帐号:");
    scanf("%d", &tmphirernum);
	printf("\t============================================================\n");

    if(!isExistedHirer(*head, tmphirernum))
    {
        printf("\t*该帐号不存在,重新输入请按1");
        scanf("%d", &renummark);

        return renummark;
    }

    return tmphirernum;
}


void returnSystem(Hirer *hirerhead, BavInfo *bavhead)
{
    int tmphirernum;
    int endreturnmark = 1;
    Hirer *p;

    printf("\t\t*---您好，欢迎进入归还系统！---*\n");
    
    tmphirernum = returnHirernum(hirerhead);
    
    if(tmphirernum == 1)
    {
        returnHirernum(hirerhead);   //重新输入帐号
    }
    else
    {
        p = isExistedHirer(*hirerhead, tmphirernum);
    }

    while(endreturnmark)
    {
        returnSomeBav(hirerhead, bavhead, p);
        printf("\t请问您是否要继续归还?继续请按1,结束请按0：");
        scanf("%d", &endreturnmark);
    }
}

int readHirerInfo(Hirer head) 
{
    FILE *hirerfp;
    Hirer *p;

    if((hirerfp = fopen("hirerinfo.dat", "rb")) == NULL)
    {
        printf("\t没有Hirer的信息,请先添加Hirer的信息!(没有打开成功)\n");
        return -1;
    }

    if(hirercount == 0)
    {
        printf("\t没有Hirer信息,请先添加Hirer信息!\n");
        
        //此处应该返回主菜单
    }
    else
    {
        for(p = head.next; p; p = p->next)
        {
            fread(p, sizeof(Hirer), 1, hirerfp);
        }
    }

    fclose(hirerfp);
    return 1;
}

void printHirersInfo(Hirer head)
{
    Hirer *p;
    int i;
    
    if(readHirerInfo(head) != -1)
    {
        for(p = head.next; p; p = p->next)
        {
			printf("\t============================================================\n");
            printf("\n姓名\t帐号\t\t电话号码\n");
            printf("\n%s\t%d\t\t%s\n", p->hirername, p->hirernum, p->hirerphonenum);
			printf("\t============================================================\n");
            
            if(p->hireramount == 0)
            {
				printf("\t============================================================\n");
                printf("\n\t您还未租借Bav!\n");
				printf("\t============================================================\n");
            }
            else
            {
				printf("\t您所租借的Bav详细信息如下:\n");
                for(i = 0; i < p->hireramount; i++)
				{
                    printf("\n\t编号\t名字\t作者\t属性\t原价\t租价\t库存\n");
                    printf("\n\t%d\t%s\t%s\t%s\t%.1f\t%.1f\t%d", p->hirebav[i].bavinfo.bavnum, 
                        p->hirebav[i].bavinfo.bavname, p->hirebav[i].bavinfo.author, 
                        p->hirebav[i].bavinfo.bavkind, p->hirebav[i].bavinfo.price, 
                        p->hirebav[i].bavinfo.bavprice, p->hirebav[i].bavinfo.inventory);

                    printf("\n\t租借的日期\t归还的日期\t过期天数\t金额\n");
                    printf("\n\t%d %d %d\t%d %d %d\t%d\t%.1f\n", p->hirebav[i].startday.year, 
                        p->hirebav[i].startday.month, p->hirebav[i].startday.day, 
                        p->hirebav[i].enday.year, p->hirebav[i].enday.month, 
                        p->hirebav[i].enday.day, p->hirebav[i].overday, p->hirebav[i].money);
                }
                printf("\t共计租借本数:");
                printf("%d\n", p->hireramount);
            }
        }
    }
}
void saveHirerInfo(Hirer head)
{
    FILE *hirerfp;
    Hirer *p;

    if((hirerfp = fopen("hirerinfo.dat", "wb")) == NULL)
    {
        printf("\t创建文件失败!\n");

        return;
    }

    for(p = head.next; p; p = p->next)
    {
        if(fwrite(p, sizeof(Hirer), 1, hirerfp) == 0)
        {
            printf("\t向文件输入数据失败!\n");
        }
    }
    
    fclose(hirerfp);
}

void hirerSomeBav(Hirer *hirerhead, BavInfo *bavhead, Hirer *p)
{
    Hirer *q = NULL;
    BavInfo *r;
    int i;
    int endmark = 1;

    char tmpbavname[30];
    char tmpauthor[20];
//    struct HireBav hirebav[5];

	system("clear");
	printf("\t============================================================\n");
    printf("\t现在开始租借Bav!\n\n");
    printf("\t每个帐号最多可以租借5本Bav!\n");
	printf("\t============================================================\n");
    
    p->hireramount = 0;

    for(i = 0; i < 5 && endmark; i++)
    {
        printf("\t请输入您要租借的Bav的名字和作者(如:小时代1 郭敬明):");
        scanf("%s", tmpbavname);
        scanf("%s", tmpauthor);

        if((r = isExisted(*bavhead, tmpbavname, tmpauthor)) != NULL)
        {
            p->hirebav[i].bavinfo.bavnum = r->bavnum;
            strcpy(p->hirebav[i].bavinfo.bavname, r->bavname);
            strcpy(p->hirebav[i].bavinfo.author, r->author);
            strcpy(p->hirebav[i].bavinfo.bavkind, r->bavkind);
            p->hirebav[i].bavinfo.price = r->price;
            p->hirebav[i].bavinfo.bavprice = r->bavprice;
            r->inventory -= 1;
            p->hirebav[i].bavinfo.inventory = r->inventory;
           // p->hirebav[i].bavinfo.inventory = --r->inventory;

            printf("\t请输入租借的日期(输入格式如:2016 1 1):");
            scanf("%d %d %d", &p->hirebav[i].startday.year, &p->hirebav[i].startday.month, &p->hirebav[i].startday.day);
            p->hirebav[i].enday.year = 0;               //这里只是租借,还没有归还,所以归还日期都为0;
            p->hirebav[i].enday.month = 0;              //过期天数也为0;
            p->hirebav[i].enday.day = 0;                //总钱数也只是租借十天的价格;
            p->hirebav[i].overday = 0;
            p->hirebav[i].money = r->bavprice;
           
			printf("\t============================================================\n\n");
            printf("\t第%d本Bav租借成功!\n", i+1);
            
            p->hireramount++;
        }
        else
        {
            printf("\t该Bav不存在或着库存为0!\n");
        }
		
		printf("\t============================================================\n");
        printf("\t继续租借请按1, 结束租借请按0：");
        scanf("%d", &endmark);
		printf("\t============================================================\n");
    }
	p->next = NULL;

    hirercount++;
    saveHirerInfo(*hirerhead);
    saveBavInfo(*bavhead);
}

int inputHirernum(Hirer *head)
{
    int renummark;
    int tmphirernum;

    printf("\t请输入您的账号:");
    scanf("%d", &tmphirernum);

    if(!isExistedHirer(*head, tmphirernum))
    {
        printf("\t该帐号不存在,重新输入请按1,重新注册帐号请按0\n");
        scanf("%d", &renummark);

        return renummark;
    }

    return tmphirernum;
}

Hirer *inputHirerInfo(Hirer *head)
{
    
    Hirer *p;
    Hirer *q;

    char hirername[20];
    char hirerphonenum[12];
//    HireBav hirebav[5];             //每个人每次只能租借5本Bav
 //   BavInfo bavinfo;
    printf("\t============================================================\n");
    printf("\t现在开始进行租借者信息录入!\n");   
	printf("\t============================================================\n");

    printf("\t***您的帐号为:%d***\n", hirernum);
    printf("\t请输入您的姓名:");
    scanf("%s", hirername);
    printf("\t请输入您的电话号码(12位):");
    scanf("%s", hirerphonenum);
	printf("\t============================================================\n\n\n");

    p = (Hirer *)Malloc(sizeof(Hirer));
    p->hirernum = hirernum;
    strcpy(p->hirername, hirername);
    strcpy(p->hirerphonenum, hirerphonenum);
    
    p->next == NULL;

    for(q = head; q->next; q = q->next)
    {
        ;
    }
    
    q->next = p;

    hirernum++;

    return p;
    saveHirerInfo(*head);
}

Hirer *isExistedHirer(Hirer head, int tmphirernum)
{
    Hirer *p;

    for(p = head.next; p; p = p->next)
    {

        if(tmphirernum == p->hirernum)
        {
            return p;
        }
    }

    return NULL;

}

void HirerSystem(Hirer *hirerhead, BavInfo *bavhead)
{
    int havemark;
    int renummark;
    Hirer *p;

    printf("\t\t*---您好，欢迎进入租借系统！---*\n");
    printf("\t============================================================\n");
    printf("\t请问您当前是否有帐号?是请按1,否请按0：");
    scanf("%d", &havemark);
    
    if(havemark == 0)
    {
        //没有租借号的情况下就开始添加租借信息
        p = inputHirerInfo(hirerhead);   
		hirerSomeBav(hirerhead, bavhead, p);
    }
    else
    {
        renummark = inputHirernum(hirerhead);
    
        if(renummark == 1)
        {
            inputHirernum(hirerhead); //重新输入帐号
        }
        else if(renummark == 0)
        {
            p = inputHirerInfo(hirerhead);
			hirerSomeBav(hirerhead, bavhead, p);
        }
        else
        {
            p = isExistedHirer(*hirerhead, renummark);
			havenumHirerSomeBav(hirerhead, bavhead, p);
        }
    }
}

void printBavInfo(BavInfo head)
{
    BavInfo *p;
    
    if(readBavInfo(head) != -1)
    {
        for(p = head.next; p; p = p->next)
        {
			printf("\t============================================================\n");
            printf("\n\t编号\t名字\t\t作者\t属性\t原价\t租价\t库存\n");
            printf("\n\t%d\t%s\t\t%s\t%s\t%.1f\t%.1f\t%d\n", p->bavnum, p->bavname, p->author, p->bavkind, p->price, p->bavprice, p->inventory);
			printf("\t============================================================\n");
        }
    }
}

int readBavInfo(BavInfo head)
{
    FILE *bavfp;
    BavInfo *p;

    if((bavfp = fopen("bavinfo.dat", "rb")) == NULL)
    {
        printf("\t没有Bav信息,请先输入Bav信息!\n");
        return -1;
    }
    
    if(bavcount == 0)
    {
        printf("\t没有Bav信息,请先输入Bav信息!\n");
    
        //此处应该返回主菜单=
    }

    else
    {
        for(p = head.next; p; p = p->next)
        {
            fread(p, sizeof(BavInfo), 1, bavfp);
        }
    }

    fclose(bavfp);

    return 1;
}

BavInfo *isExisted(BavInfo head, char bavname[30], char author[20])
{
    BavInfo *p;

    for(p = head.next; p; p = p->next)
    {
        if(!strcmp(p->bavname, bavname) && !strcmp(p->author, author))
        {
            if(p->inventory <= 0)
            {
                printf("\t该Bav库存不足,无法进行租借!\n");
                return NULL;
            }
            return p;
        }
    }
    return NULL;
}

void saveBavInfo(BavInfo head)
{
    FILE *bavfp;
    BavInfo *p; 
    
    if((bavfp = fopen("bavinfo.dat", "wb")) == NULL)
    {
        printf("\t创建文件失败!\n");

        return;
    }
    
    for(p = head.next; p; p = p->next)
    {
        if(fwrite(p, sizeof(BavInfo), 1, bavfp) == 0)
        {
            printf("\t向文件输入数据失败!\n");

        }
    }

    fclose(bavfp);
}


void inputBavInfo(BavInfo *head)
{
    BavInfo *p;
    BavInfo *q;
    BavInfo *r;         //接收isExisted返回过来的指针
    int mark = 1;       //添加结束标志
    int kind;

    char bavname[30];
    char author[20];
    char bavkind[10];
    float price;
    float bavprice;
    int inventory;
	
	printf("\t============================================================\n");
    printf("\t现在开始添加Bav的信息:\n");
    printf("\t============================================================\n");
    while(mark)
    {
        printf("\t请输入Bav的名字:");
        scanf("%s", bavname);
        printf("\t请输入作者的名字:");
        scanf("%s", author);
        if(!isExisted(*head, bavname, author))
        {
            printf("\t请选择Bav的类型(1. 图书 2. audio 3.video):");
            scanf("%d", &kind);
            switch(kind)
            {
                case 1:strcpy(bavkind, "图书");
                break;
                
                case 2:strcpy(bavkind, "audio");
                break;
     
                case 3:strcpy(bavkind, "video");
                break;
            }
            printf("\t请输入该Bav的原价:");
            scanf("%f", &price);
            bavprice = price*0.05;
            inventory = 1;

            p = (BavInfo *)Malloc(sizeof(BavInfo));
            p->bavnum = bavnum;
            strcpy(p->bavname, bavname);
            strcpy(p->author, author);
            strcpy(p->bavkind, bavkind);
            p->price = price;
            p->bavprice = bavprice;
            p->inventory = inventory;
            p->next = NULL;

            if(head->next == NULL)
            {
                head->next = p;
            }
            else
            {
                q->next = p;
            }
            q = p;

            bavnum++;
        }    
        else
        {
            //查询到这个bav的信息并给inventory + 1  
            r = isExisted(*head, bavname, author);
            r->inventory++;
        }
        bavcount++;
		printf("\t============================================================\n");
        printf("\t第%d个信息已经录入完毕,是否继续录入?\n", bavcount);
		printf("\t继续请按1，结束请按0:");
		scanf("%d", &mark);
		printf("\t============================================================\n");
        if(mark == 0)
        {
            saveBavInfo(*head);
        }
    }
}

void *Malloc(size_t size)
{
    void *result;

    result = malloc(size);
    
    if(result == NULL)
    {
        printf("\t内存申请失败!\n");
    }  

    return result;
}

int main(int argc,char *argv[])
{
    BavInfo bavhead = {0};
    Hirer hirerhead = {0};

    system("clear");
	menu(&bavhead, &hirerhead);
}
