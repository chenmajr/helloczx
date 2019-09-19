#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "song_name.h"
#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QScrollBar>
#include <pthread.h>
#include <QLabel>
#include <string.h>
#include <QListWidget>
#include <QComboBox>
#include <time.h>
#include <QTableWidget>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QPoint>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void start_music_button();
    void up_music_button();
    void down_music_button();
    void no_voice_button();
    void close_button();
    void slider_voice();
    void show_song();
    void windowpic_pic();
    void time_time();
     void label_time(int);
     void label_away(int);
    void label_voice_(int);
    void label_fife_(QString);
    void lrc_show_();
    void lrc_init(char*);
    void down_get();
    void label_lrc1_(char*);
    void label_lrc2_(char*);
    void label_lrc4_(char*);
    void label_lrc5_(char*);
    void label_person_(char *);
    void listwidget_();
    //void tablewidget_();
    void combox_init();
    void potos_init();
    void line1_init();
    void label_away_init();
    void listwidget_lrc_init();
    void get_time_(LRC*);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void player();
    void no_voice();
    void close_music();
    void voice_adjust(int);
    void down_music_();
    void up_music_();
    void adjust_time(int);
    void slider_time1();
    void on_listWidget_clicked(const QModelIndex &index);
    void combox_onChanged(int index);
    void search_name(QString);
    void return_time(const QModelIndex &index);

public:
    QTimer timer;
    FILE *fp;
    int num;
    int num_lrc;
    int num_photos;
    int num_s;
    int num_t;
    bool flag2;
    bool flag;
    bool flag1;
    bool flag3;
    int fd;
    QPushButton start_music;
    QPushButton up_music;
    QPushButton down_music;
    QPushButton no_voidce;
    QPushButton close_;
    QPushButton show_window;
    QLabel windowpic;
    QSlider slider;
    QSlider slider_time;
    QScrollBar scroll;
    char **buf;
    char **buf_lrc;
    char **buf_photos;
    char sstr[128];
    char *get_time;
    int num_song;
    QLabel label;
    QLabel label_t;
    QLabel label_voice;
    QLabel label_fife;
    pthread_mutex_t mutex;
    QLabel label_lrc;
    QLabel label_lrc1;
    QLabel label_lrc2;
    QLabel label_lrc4;
    QLabel label_lrc5;
    QLabel label_person;
    QListWidget listwidget;
    //QTableWidget *tablewidget;
    QListWidget listwidget_lrc;

    QComboBox combox;
    int com_box_page;
    QLineEdit line1;

    QPoint point;
    LRC *pb;
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
