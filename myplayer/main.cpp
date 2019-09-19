#include "mainwindow.h"
#include <QApplication>
#include "stdio.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include "fcntl.h"
#include "string.h"
#include <pthread.h>
#include <QDebug>
#include <QTextCodec>
int pipedes[2];
int num_s;
int j;
bool flag4 = true;
bool flag3 = false;
char buff[128];
void get_song_name(QString ,void *);
void get_song_title(void *arg);
void *pthread_write(void *arg)
{
    MainWindow *w = reinterpret_cast<MainWindow *>(arg);
    while(1)
    {
        usleep(100000);
        if(w->flag == true)
        {
            pthread_mutex_lock(&w->mutex);
            write(w->fd,"get_time_pos\n",strlen("get_time_pos\n"));
            pthread_mutex_unlock(&w->mutex);
            usleep(100000);
            pthread_mutex_lock(&w->mutex);
            write(w->fd,"get_time_length\n",strlen("get_time_length\n"));
            pthread_mutex_unlock(&w->mutex);
        }
    }
}
void *pthread_read(void *arg)
{
    close(pipedes[1]);
    dup2(pipedes[0],0);
    char buf[128]="";
    while(1)
    {
        fgets(buf, sizeof(buf),stdin);
        get_song_name(buf,arg);
    }
}
void get_song_name(QString buf,void *arg)
{
    MainWindow *w=reinterpret_cast<MainWindow *>(arg);
    QStringList list=buf.split('=');
    if(list[0].compare("ANS_LENGTH")==0)
    {
        QString buf;
        buf=list[1];
        QStringList list1=buf.split("\n");
        buf = list1[0];
        double a=buf.toDouble();
        int num=static_cast<int>(a);
        w->num_t = num;
        w->label_away(num);
    }
    else if(list[0].compare("ANS_TIME_POSITION")==0)
    {
        QString buf;
        buf=list[1];
        QStringList list1=buf.split("\n");
        buf = list1[0];
        double a=buf.toDouble();
        int num=static_cast<int>(a);
        num_s = num;
        w->label_time(num);
    }
    else if(list[0].compare("ANS_FILENAME")==0)
    {
        QString buf;
        buf = list[1];
        QStringList list1=buf.split("'");
        buf = list1[1];
        QStringList list2=buf.split(".mp3");
        buf = list2[0];
        buf = buf+".lrc";
        //qDebug()<<"now time:"<<buf;
        while(1)
        {
            QString str = w->buf_lrc[j];
            if(buf == str)
            {
                sprintf(buff,"/home/edu/01musicplayer/lrc/%s",w->buf_lrc[j]);
                j = 0;
                flag3 = true;
                break;
            }
            j++;
        }
        //qDebug()<<"now time:"<<buff;
    }

}
void *pthread_cut(void *arg)
{

    while(1)
    {
        usleep(100000);
        if(flag3 == true)
        {
            flag3 = false;
            get_song_title(arg);
        }
    }
}
void get_song_title(void *arg)
{
    MainWindow *w=reinterpret_cast<MainWindow *>(arg);
    LRC *pb = NULL,*pf = NULL,*p_new = NULL;
    LLIST *head = NULL;
    Create_double_link_head(&head);
    char lrc_name[128],signal[128],album[128],producer[128],*temp;
    int min = 0,sec = 0,time_buf[5];
    char *seg_lrc[128];
    usleep(100000);
    char *lrc_buf = read_lrc(buff);
    int len = lrc_strtok(lrc_buf,seg_lrc,"\r\n");//把歌词切成很多块存入数组中
    sscanf(seg_lrc[0],"%*4c%[^]]",lrc_name); //拆包,把歌词信息传入数组中
    sscanf(seg_lrc[1],"%*4c%[^]]",signal);
    sscanf(seg_lrc[2],"%*4c%[^]]",album);
    sscanf(seg_lrc[3],"%*4c%[^]]",producer);
    w->label_fife_(lrc_name);
    w->label_person_(signal);
    for(int k = 4;k < len;k++)
    {
        temp = seg_lrc[k];  //取歌词
        int i = 0;
        while(*temp == '[')
        {
            sscanf(temp,"[%2d:%2d",&min,&sec);
            time_buf[i++]=min*60+sec;
            while(*temp != ']')//如果歌词文件中没有遇到']',temp继续往下找
                temp = temp + 1;
            temp = temp + 1;//找到']'后,往后只一个,指向下一个'[',如果没有结束循环
        }
        for(int j = 0;j < i;j++)//循环开辟节点给节点赋值时间以及对应的歌词
        {			//如若一句歌词对应两个时间点或者多个,time_buf[i++]会分别存储时间
            //i是一句歌词的时间点数,j<i是对不同时间点的同一句歌词开辟多个节点存储
            p_new=(LRC*)malloc(sizeof(LRC));//申请节点开辟空间
            p_new->t_time=time_buf[j];//节点的时间成员赋值
            strcpy(p_new->src,temp);//节点的歌词成员赋值
            lrc_insert(head,p_new);//插入链表
            Order_double_link_data(head);//每次插完以时间大小进行排序
        }
    }
    pf = head->head->next;//pf指向首节点
    w->get_time_(pf);
    int i = 0;
    int a = 0;
    w->listwidget_lrc.setCurrentRow(i);
    LRC *pa = pf;
    while(1)
    {
        if(pf->next != NULL)
            pb=lrc_search_time(head,num_s);
        if(pb == NULL);
        else
            pf = pb;
        if(pf == pa)
            flag4 = true;
        for(a = 0;a < w->listwidget_lrc.count();a++)
        {
            if(strcmp(w->listwidget_lrc.item(a)->text().toLocal8Bit().data(),pf->src)==0 && flag4 == true && a>= i)
            {
                //qDebug()<<w->listwidget_lrc.item(a)->text().toLocal8Bit().data();
                //qDebug()<<pf->src;
                //qDebug()<<a;
                flag4 = false;
                if(pf->src !=  NULL)
                {
                    qDebug()<<a;
                    i = a;
                    w->listwidget_lrc.setCurrentRow(a);
                    //w->listwidget_lrc.currentItem()->setTextAlignment();
                    break;
                }
            }
        }

//        if(pf != NULL && pf->src != "" && pf == pa)
//        {
//            qDebug()<<i;
//            i++;
//        }
        pa = pf->next;
        if(flag3 == true)
        {
            w->label_lrc1.clear();
            w->label_lrc2.clear();
            w->label_lrc.clear();
            w->label_lrc4.clear();
            w->label_lrc5.clear();
            w->listwidget_lrc.clear();
            destroy_linklist(head);
            break;
        }
    }
}
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    mkfifo("cmd_fifo",0666|O_CREAT|O_TRUNC);
    MainWindow w;
    w.show();
    if(pipe(pipedes)==-1)
    {
        perror("");
        exit(-1);
    }
    pid_t pid;
    pid = fork();
    if(pid < 0)
        perror("");
    else if(pid == 0)
    {
        char song[128]="/home/edu/01musicplayer/song/听你听我.mp3";
        char buf[128] ="";
        FILE *fp = fopen("/home/edu/01musicplayer/a.txt","r");
        fread(buf,1,sizeof(buf),fp);
        fclose(fp);
        if(strcmp(buf,"") != 0)
        {
            sprintf(song,"/home/edu/01musicplayer/song/%s",buf);
        }
        close(pipedes[0]);
        dup2(pipedes[1],1);
        execlp("mplayer","mplayer","-ac","mad","-slave","-quiet","-input","file=cmd_fifo",song,NULL);

    }
    else
    {
        pthread_t pthid1,pthid2,pthid3;
        pthread_create(&pthid1,NULL,pthread_write,&w);
        pthread_create(&pthid2,NULL,pthread_read,&w);
        pthread_create(&pthid3,NULL,pthread_cut,&w);
        pthread_mutex_lock(&w.mutex);
        write(w.fd,"get_file_name\n",strlen("get_file_name\n"));
        pthread_mutex_unlock(&w.mutex);
    }
    return a.exec();
}
