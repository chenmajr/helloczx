#ifndef SONG_NAME_H
#define SONG_NAME_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
typedef struct lrc
{
    int t_time;
    char src[256];
    struct lrc *prev;
    struct lrc *next;
}LRC;
typedef struct link_list
{
    LRC *head;
    LRC *tail;
    int size;
}LLIST;
char *read_lrc(char *a);
int lrc_strtok(char *lrc_buf,char **seg_lrc,char *str);
void Create_double_link_head(LLIST **list);
void lrc_insert(LLIST *list,LRC *p_new);//尾插
void Order_double_link_data(LLIST *list);//按时间大小排序
LRC*lrc_search_time(LLIST *list,int num);
void destroy_linklist(LLIST* list);

#endif // SONG_NAME_H
