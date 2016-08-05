/*************************************************************************
	> File Name: list_test.c
	> Author: zhuyidi
	> Mail: 1311243087@qq.com
	> Created Time: 2016年08月04日 星期四 11时18分41秒
 ************************************************************************/


#include<stdio.h>
#include<malloc.h>

typedef struct NODE
{
    int data;
    struct NODE *next;
}NODE;

NODE *createList();
void showList(NODE *head);
NODE *reverseList(NODE *head);
void sortList(NODE *head);

void sortList(NODE *head)
{
    NODE tmp; 
    NODE *pi;
    NODE *pj;
    NODE *ptmp;
    NODE *phead;

    //phead.next = head;

    for(pi = head; pi; pi = pi->next)
    {
        for(pj = pi->next; pj; pj = pj->next)
        {
            if(pi->data > pj->data)
            {
                tmp = *pi;
                *pi = *pj;
                *pj = tmp;
                
                ptmp = pi->next;
                pi->next = pj->next;
                pj->next = ptmp;
            }
        }
    }
}

NODE *reverseList(NODE *head)
{
    NODE *pre = NULL;
    NODE *cur = head;
    NODE *nxt = NULL;

    if(head == NULL && head->next == NULL)
    {
        return head;
    }

    while(cur)
    {
        nxt = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nxt;
    }

    return pre;
}

void showList(NODE *head)
{
    NODE *p;

    for(p = head; p; p = p->next)
    {
        printf("%d\n", p->data);
    }
    printf("\n");
}

NODE *createList()
{
    NODE * head;
    NODE *cur;
    NODE *pre;
    int number = 5;
    int n = 1;

    if(!number)
    {
        return NULL;
    }
    
    head = (NODE *)malloc(sizeof(NODE));
    scanf("%d", &head->data);
    head->next = NULL;

    pre = head;
    cur = NULL;

    while(n++ < 5)
    {
        cur = (NODE *)malloc(sizeof(NODE));
        scanf("%d", &cur->data);
        cur->next = NULL;
        pre->next = cur;
        pre = cur;
    }

    return head;
}

int main(int argc,char *argv[])
{
    NODE *head = createList();
    printf("\n");
    head = reverseList(head);
    showList(head);
    sortList(head);
    showList(head);
}
