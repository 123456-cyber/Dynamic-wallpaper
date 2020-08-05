#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QFileDialog>
#include<QMediaPlayer>
#include <QMediaPlaylist>
#include<QVideoWidget>
#include<Windows.h>
#include<WinUser.h>
#include<QSettings>
#include<QSystemTrayIcon>
#include<QAction>
#include<QMenu>
#pragma comment(lib,"user32.lib")

//定义开机自启目录
#define AUTO_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define AUTO_PATH "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp"

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

    void on_horizontalSlider_valueChanged(int value); //调节音量

    void setAppautoRun(bool flag);                //设置程序开机自启


private:
    Ui::Widget *ui;
    QString path="";             //文件路径
    QVideoWidget *videoWidget=NULL; //播放界面实例指针
    QMediaPlayer* play;       //播放器实例指针
    QMediaPlaylist* playlist; //播放列表实例指针
    QSettings*app_set;        //应用程序设置实例指针
    QSystemTrayIcon*sys;      //系统托盘图标实例指针
    QAction* close1;           //系统托盘关闭
    QAction*about;            //关于
    QMenu *menu;              //系统托盘右键菜单
};
#endif // WIDGET_H
