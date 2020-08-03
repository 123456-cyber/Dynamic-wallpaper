#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QFileDialog>
#include<QMediaPlayer>
#include <QMediaPlaylist>
#include<QVideoWidget>
#include<Windows.h>
#include<WinUser.h>
#pragma comment(lib,"user32.lib")

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();  //选取文件

    void on_pushButton_2_clicked(); //应用动态壁纸

    void closeEvent(QCloseEvent *event);        //窗口关闭时程序退出

    void on_pushButton_3_clicked();             //取消应用动态壁纸

private:
    Ui::Widget *ui;
    QString path="";             //文件路径
    QVideoWidget *videoWidget=NULL; //播放界面实例指针
    QMediaPlayer* play;       //播放器实例指针
    QMediaPlaylist* playlist; //播放列表实例指针
};
#endif // WIDGET_H
