#ifndef MINIMAINWINDOW_H
#define MINIMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
class Minimainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Minimainwindow(QMainWindow *parent = nullptr);
    void back_init();
    void bgpic_init(char *str);
    void label_init();
    void label1_(char *str);
    void label2_(char *str);
    void label3_(char *str);
    void label4_(char *str);
    void label5_(char *str);
    void tatil_(char *str);
    void picture_(int num);
    void potos_init();
    QLabel label1;
    QLabel label2;
    QLabel label3;
    QLabel label4;
    QLabel label5;
public:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
    void signal_();

private:
    QPushButton back;
    QPoint point;
    QLabel bgpic;
    QLabel tatil;
    QLabel picture;
    QTimer timer;
    char **buf_photos;
    int num_photos;
    int num;
public slots:
    void set_signal();
    void aoto_pic();
};

#endif // MINIMAINWINDOW_H
