#include "song_name.h"
#include <QDebug>

void Create_double_link_head(LLIST **list)//创建头节点,用来连接歌词子节点
{
    LRC *header = (LRC*)malloc(sizeof(LRC));
    header->t_time = 0;
    header->prev = NULL;
    header->next = NULL;
    LLIST *temp = (LLIST*)malloc(sizeof(LLIST));
    temp->head = header;
    temp->tail = header;
    temp->size = 0;
    *list = temp;
}
char *read_lrc(char *a) //将lrc歌词文件读出来,存在文字常量区,并用字符指针lrc指向该区的首地址
{
    FILE *fp = fopen(a,"rb");
    if(!fp)
    {
        perror("");
        return 0;
    }
    char *lrc;
    unsigned long int lenth = 0;
    fseek(fp,0,SEEK_END);
    lenth = ftell(fp);
    rewind(fp);
    lrc =  (char *)malloc(lenth+1);
    fread(lrc,lenth,1,fp);
    lrc[lenth] = '\0';
    fclose(fp);
    return lrc;  //返回lrc指向的首地址
}
int lrc_strtok(char *lrc_buf,char **seg_lrc,char *str)//将lrc_buf里面的歌词拿出来,遇到\r\n进行切割
{
    int i = 0;
    seg_lrc[i] = strtok(lrc_buf,str);
    while(seg_lrc[i] != NULL)//循环切割,然后放到数组seg_lrc中
        seg_lrc[++i] = strtok(NULL,str);
    return i;//返回切割的块数
}
void lrc_insert(LLIST *list,LRC *p_new)//将新来的节点尾插
{
    if( list->tail == NULL)
        return;
    LRC *tail = list->tail;
    tail->next = p_new;
    p_new->prev = tail;
    p_new->next = NULL;
    list->tail = p_new;
    list->size++;
}
void Order_double_link_data(LLIST *list)//按时间大小排序
{
    if(list == NULL || list->head == NULL || list->head->next == NULL)
        return ;
    LRC *p = list->head->next;
    LRC *q = NULL;
    for(p;p->next != NULL;p = p->next)
    {
        for(q = p->next;q != NULL;q = q->next)
        {
            if(p->t_time > q->t_time)
            {
                LRC temp = *q;
                *q  =  *p;
                *p  =  temp;

                temp.next = q->next;
                q->next = p->next;
                p->next = temp.next;

                temp.prev = q->prev;
                q->prev = p->prev;
                p->prev = temp.prev;
            }
        }
    }
}
LRC*lrc_search_time(LLIST *list,int num)//按时间进行查找
{
    LRC*p_mov;
    p_mov=list->head->next;//指向首节点
    while(p_mov!=NULL)
    {
        if(p_mov->t_time==num)//找到时间匹配上的了
        {
            return p_mov;  //返回地址
        }
        p_mov=p_mov->next;
    }
    return NULL;
}
void destroy_linklist(LLIST* list)
{
    LRC *head = list->head;
    LRC *tmp=NULL;
    while( head != NULL)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
    free(list);
}
