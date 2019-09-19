#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include "fcntl.h"
#include "song_name.h"
#include <QPushButton>
#include <dirent.h>
#include <QDebug>
#include <QStringList>
#include <QEvent>
#include <QColor>
#include <QListWidgetItem>
#include <QMouseEvent>
#include<QPainter>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    flag = true;
    flag1 = true;
    flag3 = true;
    num_song = 0;
    num_s = 0;
    num_t = 500;
    flag2 = false;
    com_box_page = 0;
    label_voice.hide();
    pthread_mutex_init(&mutex,NULL);
    fd = open("cmd_fifo",O_RDWR);
    pthread_mutex_lock(&mutex);
    write(fd,"get_time_length\n",strlen("get_time_length\n"));
    pthread_mutex_unlock(&mutex);
    this->setFixedSize(1024,600);
    windowpic_pic();

    label.setParent(this);
    label.setStyleSheet("color:black;");
    label.setGeometry(188,558,40,20);//
    label_away_init();

    start_music_button();
    up_music_button();
    down_music_button();
    no_voice_button();
    close_button();
    slider_voice();
    combox_init();//设置下拉框
    time_time();
    label_time(NULL);
    label_away(NULL);
    show_song();
    lrc_show_();
    potos_init();
    listwidget_();
    listwidget_lrc_init();
    label_voice_(50);

    //label_fife_("");
    label_fife.setParent(this);
    label_fife.setStyleSheet("color:red;");
    label_fife.setGeometry(200,30,300,40);
    label_fife.setAlignment(Qt::AlignHCenter);

    line1_init();
    connect(&combox,SIGNAL(currentIndexChanged(int)),this,SLOT(combox_onChanged(int)));
    connect(&start_music,SIGNAL(clicked()),this,SLOT(player()));
    connect(&no_voidce,SIGNAL(clicked()),this,SLOT(no_voice()));
    connect(&close_,SIGNAL(clicked()),this,SLOT(close_music()));
    connect(&slider,SIGNAL(valueChanged(int)),this,SLOT(voice_adjust(int)));
    connect(&down_music,SIGNAL(clicked()),this,SLOT(down_music_()));
    connect(&up_music,SIGNAL(clicked()),this,SLOT(up_music_()));
    connect(&slider_time,SIGNAL(sliderPressed()),this,SLOT(slider_time1()));
    connect(&slider_time,SIGNAL(valueChanged(int)),this,SLOT(adjust_time(int)));
    connect(&listwidget,SIGNAL(clicked(QModelIndex)),this,SLOT(on_listWidget_clicked(QModelIndex)));
    //connect(tablewidget,SIGNAL(cellClicked(int,int)),this,SLOT(getItem(int,int)));
    connect(&show_window,SIGNAL(clicked()),this,SLOT(show_window_()));
    connect(&line1,SIGNAL(textChanged(QString)),this,SLOT(search_name(QString)));
    connect(&listwidget_lrc,SIGNAL(clicked(QModelIndex)),this,SLOT(return_time(QModelIndex)));
}

void MainWindow::start_music_button()
{
    start_music.setParent(this);
    start_music.setText("");
    start_music.setFlat(true);
    start_music.resize(40,40);
    start_music.move(85,545);
    start_music.setIcon(QPixmap(":/new/prefix1/image/end1.png"));
    start_music.setIconSize(QSize(40,40));
}
void MainWindow::up_music_button()
{
    up_music.setParent(this);
    up_music.setText("");
    up_music.setFlat(true);
    up_music.resize(40,40);
    up_music.move(45,545);
    up_music.setIcon(QPixmap(":/new/prefix1/image/up.png"));
    up_music.setIconSize(QSize(40,40));
}

void MainWindow::down_music_button()
{
    down_music.setParent(this);
    down_music.setText("");
    down_music.setFlat(true);
    down_music.resize(40,40);
    down_music.move(125,545);
    down_music.setIcon(QPixmap(":/new/prefix1/image/down.png"));
    down_music.setIconSize(QSize(40,40));
}

void MainWindow::no_voice_button()
{
    no_voidce.setParent(this);
    no_voidce.hide();
    no_voidce.setText("");
    no_voidce.setFlat(true);
    no_voidce.setGeometry(0,431,30,30);//音量
    no_voidce.setIcon(QPixmap(":/new/prefix1/image/voice_2.png"));
    no_voidce.setIconSize(QSize(30,30));
}

void MainWindow::close_button()
{
    close_.setParent(this);
    close_.hide();
    close_.setText("");
    close_.setFlat(true);
    close_.setGeometry(0,0,30,30);
    close_.setIcon(QPixmap(":/new/prefix1/image/close.png"));
    close_.setIconSize(QSize(30,30));

    show_window.setParent(this);
    show_window.setText("");
    show_window.setFlat(true);
    show_window.setGeometry(915,540,30,30);//540
    show_window.setIcon(QPixmap(":/new/prefix1/image/back.png"));
    show_window.setIconSize(QSize(30,30));
}
void MainWindow::line1_init()
{
    line1.setParent(this);
    line1.setGeometry(500,0,200,20);
}
void MainWindow::search_name(QString  text)
{
    QString str = text;
    if(str.compare("")!=0)
    {
        flag3 = false;
        listwidget.clear();
        for(int i = 0;i < num;i++)
        {
            char str1[128]="";
            QString p = QString(buf[i]);
            if(p.contains(str,Qt::CaseSensitive)==true)
            {
                sprintf(str1,"%s",buf[i]);
                QListWidgetItem *item=NULL;
                item=new QListWidgetItem(tr(str1),&listwidget);
                item->setTextColor(QColor(0,0,0,255));
                //item->backgroundColor(0,255,0);
                item->setBackgroundColor(QColor(0,241,240,0));
            }
        }
    }
    else
    {
        flag3 = true;
        listwidget.clear();
        char str[128]="";
        //QStringList list;
        for(int i = 0;i < num;i++)
        {
            sprintf(str,"%d %s",i+1,buf[i]);
            //list<<tr(str);
            QListWidgetItem *item=NULL;
            item=new QListWidgetItem(tr(str),&listwidget);
            item->setTextColor(QColor(0,0,0,255));
            //item->backgroundColor(0,255,0);
            item->setBackgroundColor(QColor(0,241,240,0));
        }
    }
}

void MainWindow::slider_voice()
{
    slider.setParent(this);
    slider.hide();
    slider.setMinimum(0);
    slider.setMaximum(100);
    slider.setValue(50);
    slider.setGeometry(5,480,20,100);//
    pthread_mutex_lock(&mutex);
    write(fd,"volume 50 1\n",strlen("volume 50 1\n"));
    pthread_mutex_unlock(&mutex);
    slider.setStyleSheet("QSlider::groove:vertical {border: 1px solid #4A708B;background: #C0C0C0;width: 5px;border-radius: 1px;padding-left:-1px;padding-right:-1px;padding-top:-1px;padding-bottom:-1px;}QSlider::sub-page:vertical {background: #575757;border: 1px solid #4A708B;border-radius: 2px;}QSlider::add-page:vertical {background: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 #c4c4c4, stop:1 #B1B1B1);background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,stop: 0 #5DCCFF, stop: 1 #1874CD);border: 0px solid #777;width: 10px;border-radius: 2px;}QSlider::handle:vertical {background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));height: 11px;margin-left: -3px;margin-right: -3px;border-radius: 5px;}QSlider::sub-page:vertical:disabled {background: #00009C;border-color: #999;}QSlider::add-page:vertical:disabled {background: #eee;border-color: #999;}QSlider::handle:vertical:disabled {background: #eee;border: 1px solid #aaa;border-radius: 4px;}");
}

void MainWindow::show_song()
{
    buf = new char*[1024];
    DIR *dir = NULL;
    struct dirent *namelist=NULL;
    dir = opendir("/home/edu/01musicplayer/song");
    if(dir == NULL)
        perror("");
     num = 0;
    while(1)
    {
        namelist = readdir(dir);
        if(namelist == NULL)
            break;
        if(strcmp(namelist->d_name,".")==0 || strcmp(namelist->d_name,"..")==0)
            continue;
        buf[num++] = namelist->d_name;
    }
}


void MainWindow::windowpic_pic()
{
    windowpic.setParent(this);
    windowpic.setText("");
    windowpic.resize(this->width(),this->height());
    windowpic.setPixmap(QPixmap(":/new/prefix1/image/bgm.jpg"));
    windowpic.setScaledContents(true);
}

void MainWindow::time_time()
{
    slider_time.setParent(this);
    slider_time.setTracking(false);
    slider_time.setOrientation(Qt::Horizontal);
    //slider_time.setStyleSheet("QSlider::sub-page:horizontal{height:12px; left:5px; right:5px;border-image: url(:/new/prefix1/image/blue.png);}QSlider::handle:horizontal{border-radius:20px; width:40px; height:40px; margin-top:-10px; margin-left:-10px; margin-bottom:-20px; margin-right:-10px; border-image:url(:/new/prefix1/image/handle.png);}QSlider::groove:horizontal{height:12px; left:5px; right:5px; border-image: url(:/new/prefix1/image/black.png);}");
    slider_time.setStyleSheet("QSlider::groove:horizontal {border: 1px solid #4A708B;background: #C0C0C0;height: 5px;border-radius: 1px;padding-left:-1px;padding-right:-1px;}QSlider::sub-page:horizontal {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,stop: 0 #5DCCFF, stop: 1 #1874CD);border: 1px solid #4A708B;height: 10px;border-radius: 2px;}QSlider::add-page:horizontal {background: #575757;border: 0px solid #777;height: 10px;border-radius: 2px;}QSlider::handle:horizontal {background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));width: 11px;margin-top: -3px;margin-bottom: -3px;border-radius: 5px;}QSlider::handle:horizontal:hover {background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA, stop:0.778409 rgba(255, 255, 255, 255));width: 11px;margin-top: -3px;margin-bottom: -3px;border-radius: 5px;}QSlider::sub-page:horizontal:disabled {background: #00009C;border-color: #999;}QSlider::add-page:horizontal:disabled {background: #eee;border-color: #999;}QSlider::handle:horizontal:disabled {background: #eee;border: 1px solid #aaa;border-radius: 4px;}");
    slider_time.setMinimum(0);
    slider_time.setGeometry(217,540,450,50);//slide
}

void MainWindow::label_time(int num)
{
    if(flag2 == false)
    {
        num_s = num;
        slider_time.setMaximum(num_t);
        slider_time.setValue(num);
        char buf[128]="";
        int i = num/60;
        int j = num%60;
        sprintf(buf,"%01d:%02d",i,j);
        label.setText(buf);
    }

}
void MainWindow::label_away_init()
{
    label_t.setParent(this);
    label_t.setStyleSheet("color:black;");
    label_t.setGeometry(671,558,40,20);//sj

    label_lrc.setParent(this);
    label_lrc.setStyleSheet("color:red;");
    label_lrc.setGeometry(200,195,300,20);
    label_lrc.setAlignment(Qt::AlignHCenter);
    label_lrc.hide();

    label_lrc1.setParent(this);
    label_lrc1.setStyleSheet("color:black;");
    label_lrc1.setGeometry(200,145,300,20);
    label_lrc1.setAlignment(Qt::AlignHCenter);
    label_lrc1.hide();

    label_lrc2.setParent(this);
    label_lrc2.setStyleSheet("color:black;");
    label_lrc2.setGeometry(200,170,300,20);
    label_lrc2.setAlignment(Qt::AlignHCenter);
    label_lrc2.hide();

    label_lrc4.setParent(this);
    label_lrc4.setStyleSheet("color:black;");
    label_lrc4.setGeometry(200,220,300,20);
    label_lrc4.setAlignment(Qt::AlignHCenter);
    label_lrc4.hide();

    label_lrc5.setParent(this);
    label_lrc5.setStyleSheet("color:black;");
    label_lrc5.setGeometry(200,245,300,20);
    label_lrc5.setAlignment(Qt::AlignHCenter);
    label_lrc5.hide();

    label_person.setParent(this);
    label_person.setStyleSheet("color:blue;");
    label_person.setGeometry(200,50,300,40);
    label_person.setAlignment(Qt::AlignHCenter);

}

void MainWindow::label_voice_(int vaule)
{
    QString str = QString::number(vaule,10);
    label_voice.setParent(this);
    label_voice.setStyleSheet("color:black;");
    label_voice.setGeometry(43,492,40,20);//音量大小
    label_voice.setText(str);
}

void MainWindow::label_away(int num)
{
    char buf[128]="";
    int i = num/60;
    int j = num%60;
    sprintf(buf,"%d:%02d",i,j);
    label_t.setText(buf);
}

void MainWindow::label_fife_(QString text)
{
    label_fife.setText(text);
}
void MainWindow::player()
{

    if(flag == true)
    {
        start_music.setIcon(QPixmap(":/new/prefix1/image/start1.png"));
        flag = false;
        pthread_mutex_lock(&mutex);
        write(fd,"pause\n",strlen("pause\n"));
        usleep(100000);
        pthread_mutex_unlock(&mutex);
    }
    else if(flag == false)
    {
        start_music.setIcon(QPixmap(":/new/prefix1/image/end1.png"));
        flag = true;
        pthread_mutex_lock(&mutex);
        write(fd,"pause\n",strlen("pause\n"));
        pthread_mutex_unlock(&mutex);
        voice_adjust(slider.value());
    }
}

void MainWindow::no_voice()
{
    if(flag1 == true)
    {
        no_voidce.setIcon(QPixmap(":/new/prefix1/image/no_voice.png"));
        pthread_mutex_lock(&mutex);
        write(fd,"mute 1\n",strlen("mute 1\n"));
        pthread_mutex_unlock(&mutex);
        flag1 = false;
    }
    else if(flag1 == false)
    {
        if(slider.value() > 60)
            no_voidce.setIcon(QPixmap(":/new/prefix1/image/no_2333.png"));
        else if(slider.value() <= 60 && slider.value() > 30)
            no_voidce.setIcon(QPixmap(":/new/prefix1/image/voice_2.png"));
        else if(slider.value() <= 30 && slider.value() > 0)
            no_voidce.setIcon(QPixmap(":/new/prefix1/image/voice.png"));
        else if(slider.value() == 0)
       {
            //no_voidce.setIcon(QPixmap(":/new/prefix1/image/no_voice.png"));
            slider.setValue(70);
        }
        pthread_mutex_lock(&mutex);
        write(fd,"mute 0\n",strlen("mute 0\n"));
        if(flag == false)
            write(fd,"pause\n",strlen("pause\n"));
        pthread_mutex_unlock(&mutex);
        flag1 = true;
    }
}

void MainWindow::close_music()
{
    write(fd,"q\n",strlen("q\n"));
    this->close();
}

void MainWindow::voice_adjust(int value)
{
    label_voice_(value);
    if(value > 60)
        no_voidce.setIcon(QPixmap(":/new/prefix1/image/no_2333.png"));
    else if(value <= 30 && value > 0)
        no_voidce.setIcon(QPixmap(":/new/prefix1/image/voice.png"));
    else if(value <= 60 && value > 30)
        no_voidce.setIcon(QPixmap(":/new/prefix1/image/voice_2.png"));
    else if(value == 0)
        no_voidce.setIcon(QPixmap(":/new/prefix1/image/no_voice.png"));
    if(flag1 == false)
    {
        no_voidce.setIcon(QPixmap(":/new/prefix1/image/no_2333.png"));
        pthread_mutex_lock(&mutex);
        write(fd,"mute 0\n",strlen("mute 0\n"));
        pthread_mutex_unlock(&mutex);
        flag1 = true;
    }
    QString str = QString::number(value,10);
    str = "volume "+str+" 1\n";
    if(flag == true)
    {
        pthread_mutex_lock(&mutex);
        write(fd,str.toLatin1().data(),str.size());
        pthread_mutex_unlock(&mutex);
    }
}

void MainWindow::down_music_()
{
    srand(time(NULL));
    flag2 = false;
    char str[1024] = "";
    if(com_box_page == 0)
    {
        num_song++;
    }
    else if(com_box_page == 1)
    {
        num_song = rand()%num;
    }
    if(num_song > num-1)
    {
        num_song = 0;
    }
    if(buf[num_song] != NULL)
    {
        sprintf(str,"%s%s%s","loadfile /home/edu/01musicplayer/song/",buf[num_song],"\n");
        pthread_mutex_lock(&mutex);
        write(fd,str,strlen(str));
        pthread_mutex_unlock(&mutex);
    }
    if(flag == false)
    {
        start_music.setIcon(QPixmap(":/new/prefix1/image/end1.png"));
        flag = true;
    }
    listwidget.setCurrentRow(num_song);
    //sprintf(sstr,"/home/edu/01musicplayer/photos/%s",buf_photos[num_song%num_photos]);
    //windowpic.setPixmap(QPixmap(sstr));
    down_get();
}
void MainWindow::down_get()
{
    fp = fopen("/home/edu/01musicplayer/a.txt","w");
    fwrite(buf[num_song],1,strlen(buf[num_song]),fp);
    fclose(fp);
    pthread_mutex_lock(&mutex);
    write(fd,"pause\n",strlen("pause\n"));
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&mutex);
    write(fd,"get_file_name\n",strlen("get_file_name\n"));
    pthread_mutex_unlock(&mutex);
}

void MainWindow::up_music_()
{
    srand(time(NULL));
    char str[1024] = "";
    if(com_box_page == 0)
    {
        num_song--;
    }
    else if(com_box_page == 1)
    {
        num_song = rand()%num;
    }
    if(num_song < 0)
    {
        num_song = num-1;
    }
    if(buf[num_song] != NULL)
    {
        sprintf(str,"%s%s%s","loadfile /home/edu/01musicplayer/song/",buf[num_song],"\n");
        pthread_mutex_lock(&mutex);
        write(fd,str,strlen(str));
        pthread_mutex_unlock(&mutex);
    }
    if(flag == false)
    {
        start_music.setIcon(QPixmap(":/new/prefix1/image/end1.png"));
        flag = true;
    }
    listwidget.setCurrentRow(num_song);
    pthread_mutex_lock(&mutex);
    write(fd,"pause\n",strlen("pause\n"));
    pthread_mutex_unlock(&mutex);
    //write(fd,"get_time_length\n",strlen("get_time_length\n"));
    //sleep(1);
    pthread_mutex_lock(&mutex);
    write(fd,"get_file_name\n",strlen("get_file_name\n"));
    pthread_mutex_unlock(&mutex);
    sprintf(sstr,"/home/edu/01musicplayer/photos/%s",buf_photos[num_song%num_photos]);
    //windowpic.setPixmap(QPixmap(sstr));
    fp = fopen("/home/edu/01musicplayer/a.txt","w");
    fwrite(buf[num_song],1,strlen(buf[num_song]),fp);
    fclose(fp);
}
void MainWindow::slider_time1()
{
    flag2 = true;
}

void MainWindow::adjust_time(int value)
{
    char str[1024]="";
    usleep(100000);
    if(value >= num_t-2)
    {
        down_music_();
    }
     if(flag2 == true)
    {
        if(num_s != value)
        {
            value = value - num_s;
            sprintf(str,"%s%d%s","seek ",value,"\n");
            pthread_mutex_lock(&mutex);
            write(fd,str,strlen(str));
            pthread_mutex_unlock(&mutex);
        }
        usleep(100000);
        flag2 = false;
    }
}
void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    char str[1024] = "";
    QString str1= "";
    int set = listwidget.currentRow();
    num_song = set;
    if(flag3 == false)
    {
        str1 = listwidget.item(set)->text();
        sprintf(str,"%s%s%s","loadfile /home/edu/01musicplayer/song/",str1.toLocal8Bit().data(),"\n");
    }
    if(flag3 == true)
        sprintf(str,"%s%s%s","loadfile /home/edu/01musicplayer/song/",buf[set],"\n");
    pthread_mutex_lock(&mutex);
    write(fd,str,strlen(str));
    pthread_mutex_unlock(&mutex);
    sprintf(sstr,"/home/edu/01musicplayer/photos/%s",buf_photos[set%num_photos]);
   // windowpic.setPixmap(QPixmap(sstr));
    if(flag == false)
    {
        start_music.setIcon(QPixmap(":/new/prefix1/image/end1.png"));
        flag = true;

    }
    down_get();
}

void MainWindow::combox_onChanged(int index)
{
    com_box_page = index;
}
void MainWindow::lrc_show_()
{
    buf_lrc = new char*[1024];
    DIR *dir = NULL;
    struct dirent *namelist=NULL;
    dir = opendir("/home/edu/01musicplayer/lrc");
    if(dir == NULL)
        perror("");
    num_lrc = 0;
    while(1)
    {
        namelist = readdir(dir);
        if(namelist == NULL)
            break;
        if(strcmp(namelist->d_name,".")==0 || strcmp(namelist->d_name,"..")==0)
            continue;
        buf_lrc[num_lrc++] = namelist->d_name;
    }
}
void MainWindow::lrc_init(char *str)
{
    label_lrc.setText(str);
}
void MainWindow::label_person_(char*str)
{
    label_person.setText(str);
}

void MainWindow::label_lrc1_(char*str)
{
    label_lrc1.setText(str);
}
void MainWindow::label_lrc2_(char*str)
{
    label_lrc2.setText(str);
}
void MainWindow::label_lrc4_(char*str)
{
    label_lrc4.setText(str);
}
void MainWindow::label_lrc5_(char*str)
{
    label_lrc5.setText(str);
}

void MainWindow::listwidget_()
{
    char str[128]="";
    //QStringList list;
    for(int i = 0;i < num;i++)
    {
        sprintf(str,"%d %s",i+1,buf[i]);
        //list<<tr(str);
        QListWidgetItem *item=NULL;
        item=new QListWidgetItem(tr(str),&listwidget);
        item->setTextColor(QColor(0,0,0,255));
        //item->backgroundColor(0,255,0);
        item->setBackgroundColor(QColor(0,241,240,0));
    }
    listwidget.setParent(this);
    listwidget.setFrameShape(QListWidget::NoFrame);
    listwidget.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listwidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listwidget.hide();
    listwidget.setGeometry(500,20,220,200);
    listwidget.setStyleSheet("background-color:transparent");
    char buff[128]="";
    fp = fopen("/home/edu/01musicplayer/a.txt","r");
    if(!fp)
    {
        fp = fopen("/home/edu/01musicplayer/a.txt","w");
    }
    fread(buff,1,sizeof(buff),fp);
    fclose(fp);
    if(strcmp(buff,"") != 0)
    {
        int i = 0;
        while(1)
        {
            if(strcmp(buff,buf[i])==0)
                break;
            else
                i++;
        }
        listwidget.setCurrentRow(i);
        num_song = i;
        sprintf(sstr,"/home/edu/01musicplayer/photos/%s",buf_photos[i]);
        windowpic.setPixmap(QPixmap(sstr));
    }
    else
    {
        listwidget.setCurrentRow(num_song);
        windowpic.setPixmap(QPixmap(":/new/prefix1/image/bgm.jpg"));
    }
}
void MainWindow::listwidget_lrc_init()
{
    listwidget_lrc.setParent(this);
    listwidget_lrc.setFrameShape(QListWidget::NoFrame);
    listwidget_lrc.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listwidget_lrc.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listwidget_lrc.setGeometry(200,90,300,200);//歌词位置
    listwidget_lrc.setStyleSheet("background-color:transparent");
}

void MainWindow::combox_init()
{
    combox.setParent(this);
    combox.setGeometry(954,537,45,30);//combox.setGeometry(954,552,45,30);
    QStringList list;
    list<<""<<""<<"";
    combox.addItems(list);
    combox.setItemIcon(0,QIcon(":/new/prefix1/image/comboxitem2.png"));
    combox.setItemIcon(1,QIcon(":/new/prefix1/image/comboxitem1.png"));
    combox.setItemIcon(2,QIcon(":/new/prefix1/image/comboxitem3.png"));
    //combox.setStyleSheet("QComboBox {background: transparent;}QComboBox::drop-down {background: transparent;}QComboBox::drop-down:hover {background:transparent;}QComboBox QAbstractItemView { background: transparent;}QComboBox QAbstractItemView::item:selected {background:transparent;}");
}
void MainWindow::enterEvent(QEvent *event)
{

    slider.show();
    label_voice.show();
    no_voidce.show();
    close_.show();
    listwidget.show();
}
void MainWindow::leaveEvent(QEvent *event)
{
    slider.hide();
    label_voice.hide();
    no_voidce.hide();
    close_.hide();
    listwidget.hide();
}
void MainWindow::potos_init()
{
    buf_photos = new char*[1024];
    DIR *dir = NULL;
    struct dirent *namelist=NULL;
    dir = opendir("/home/edu/01musicplayer/photos");
    if(dir == NULL)
        perror("");
    num_photos = 0;
    while(1)
    {
        namelist = readdir(dir);
        if(namelist == NULL)
            break;
        if(strcmp(namelist->d_name,".")==0 || strcmp(namelist->d_name,"..")==0)
            continue;

        buf_photos[num_photos++] = namelist->d_name;
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x() < 400 && event->x() > 30 && event->y() < 40 )
        this->move(event->globalPos() - point);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint p;
   if(event->button()==Qt::LeftButton)
       {
           p=event->globalPos()-this->frameGeometry().topLeft();
       }
  // qDebug()<<event->pos();
    if(event->x() < 400 && event->x() > 30 && event->y() < 40)
        {
            if(event->button() == Qt::LeftButton)
            {
                point = event->globalPos() - this->geometry().topLeft();
            }
    }
}
void MainWindow::return_time(const QModelIndex &index)
{
    //qDebug()<<"111111111111111111";
    LRC *pf = pb;
    get_time = listwidget_lrc.currentItem()->text().toLocal8Bit().data();
    //qDebug()<<get_time;
    while(pf->next != NULL)
    {
        if(strcmp(pf->src,get_time)==0)
        {
            qDebug()<<pf->src;
            break;
        }
        pf = pf->next;
    }
    //qDebug()<<pf->src;
    if(pf != NULL)
    {
        int a = pf->t_time - num_s;
        //qDebug()<<a;
        char str[128] = "";
        sprintf(str,"seek %d\n",a);
        pthread_mutex_lock(&mutex);
        write(fd,str,strlen(str));
        pthread_mutex_unlock(&mutex);
    }
}
void MainWindow::get_time_(LRC *pf)
{
   pb = pf;
   char str[128]="";
   while(pf != NULL)
   {
       sprintf(str,"%s",pf->src);
       //list<<tr(str);
       QListWidgetItem *item=NULL,*item1=NULL;
       //item->setTextAlignment(Qt::AlignVCenter);
       item=new QListWidgetItem(QObject::tr(str),&listwidget_lrc);
       item->setTextColor(QColor(0,0,0,255));
       item->setBackgroundColor(QColor(0,241,240,0));

       item1=new QListWidgetItem(QObject::tr(""),&listwidget_lrc);
       item1->setTextColor(QColor(0,0,0,255));
       item1->setBackgroundColor(QColor(0,241,240,0));
       pf = pf->next;
   }
}

MainWindow::~MainWindow()
{
    delete ui;
}
