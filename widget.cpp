#include "widget.h"
#include "ui_widget.h"
#include<QMessageBox>
#include<QCloseEvent>
#include<QFile>
#include<QEvent>


inline BOOL CALLBACK EnumWindowsProc(_In_ HWND tophandle, _In_ LPARAM topparamhandle);
HWND GetWorkerW();

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    play=new QMediaPlayer();
    playlist=new QMediaPlaylist();


    //设置是否开机自启动   默认自启动
    ui->checkBox_2->setCheckState(Qt::Checked);
    setAppautoRun(true);
    connect(ui->checkBox_2,&QCheckBox::stateChanged,this,&Widget::setAppautoRun);

    //程序运行时检索项目配置文件
    QFile file("./项目配置文件.ini");
    file.open(QIODevice::ReadOnly);
    char buf[200]{0};
    file.readLine(buf,199);
    file.close();
    path=buf;

    if(path.split('=')[1]!="1")
    {
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(path.split('=')[1]));
        //创建壁纸窗口实例
        videoWidget=new QVideoWidget();
        //设置窗口为无边框窗口，视频大小可缩放，全屏播放
        videoWidget->setWindowFlag(Qt::FramelessWindowHint);
        videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
        play->setVideoOutput(videoWidget);
        play->setPlaylist(playlist);
        videoWidget->setFullScreen(true);
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        SetParent((HWND)videoWidget->winId(), GetWorkerW());
        play->play();
        //播放完将进度条设置为最初位置
        connect(play,&QMediaPlayer::positionChanged,[&](qint64 t){
            if(t>=play->duration())
            {
                play->setPosition(0);
                play->play();
            }
        });
        videoWidget->show();
        path="";
    }

    //设置声音默认为开启
    ui->checkBox->setCheckState(Qt::Checked);
    //默认音量为50%
    play->setVolume(50);
    ui->horizontalSlider->setValue(50);

    //开启/关闭声音
    connect(ui->checkBox,&QCheckBox::stateChanged,[this](int state){
        if(state==Qt::Checked)
        {
            play->setMuted(false);
        }
        else
        {
            play->setMuted(true);
        }
    });

    //最小化到系统托盘  设置图标
    sys=new QSystemTrayIcon(this);
    sys->setToolTip("动态壁纸");
    sys->setIcon(QIcon(":/test.jpg"));

    close1=new QAction("退出",this);
    connect(close1,&QAction::triggered,[this](bool checked){
        if(QMessageBox::information(this,"提示","关闭该界面后动态壁纸也会随之消失,是否继续？",QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Ok)
        {
            if(videoWidget!=NULL)
            {
                videoWidget->close();
            }
            this->close();
        }
    });

    about=new QAction("关于",this);
    connect(about,&QAction::triggered,[this](bool checked){
        QMessageBox::about(0,"关于","程序：动态壁纸引擎\r\n作者：听风\r\n版本：1.5");
    });

    connect(sys,&QSystemTrayIcon::activated,[this](QSystemTrayIcon::ActivationReason reason){
        if(reason==QSystemTrayIcon::DoubleClick)
        {
            this->show();
        }
    });
    menu=new QMenu(this);
    menu->addAction(about);
    menu->addAction(close1);

    sys->setContextMenu(menu);
    sys->show();
}

Widget::~Widget()
{
    delete ui;
}

HWND window = NULL;
//遍历窗口句柄
inline BOOL CALLBACK EnumWindowsProc(_In_ HWND tophandle, _In_ LPARAM topparamhandle)
{
    HWND defview = FindWindowEx(tophandle, 0, L"shELLDLL_DefView", NULL);
    if (defview != NULL)
    {
        window = FindWindowEx(0, tophandle, L"WorkerW", 0);
    }
    return true;
}
//获取底层窗口句柄
HWND GetWorkerW()
{
    int result;
    HWND windowHandle = FindWindow(L"Progman",NULL );
    SendMessageTimeout(windowHandle, 0x052c, 0 ,0, SMTO_NORMAL, 0x3e8,(PDWORD_PTR)&result);
    EnumWindows(EnumWindowsProc,(LPARAM)nullptr);
    ShowWindow(window,SW_HIDE);
    return windowHandle;
}

void Widget::on_pushButton_clicked()
{
    path=QFileDialog::getOpenFileName(this,"打开文件","C:/Users/86159/Desktop");
    ui->lineEdit->setText(path);
    //导入目标文件
    playlist->clear();
    playlist->addMedia(QUrl::fromLocalFile(path));
}

void Widget::on_pushButton_2_clicked()
{
    if(path!="")
    {
        //清空路径
        ui->lineEdit->clear();
        //创建壁纸窗口实例
        videoWidget=new QVideoWidget();
        //设置窗口为无边框窗口，视频大小可缩放，全屏播放
        videoWidget->setWindowFlag(Qt::FramelessWindowHint);
        videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
        play->setVideoOutput(videoWidget);
        play->setPlaylist(playlist);
        videoWidget->setFullScreen(true);
        //循环播放
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        //设置播放窗口为桌面
        SetParent((HWND)videoWidget->winId(), GetWorkerW());
        play->play();
        //播放完将进度条设置为最初位置
        connect(play,&QMediaPlayer::positionChanged,[&](qint64 t){
            if(t>=play->duration())
            {
                play->setPosition(0);
                play->play();
            }
        });
        videoWidget->show();
        //应用后路径写入配置文件
        QFile file("./项目配置文件.ini");
        file.open(QIODevice::WriteOnly);
        QString temp="path="+path;
        file.write(temp.toUtf8().data(),temp.toUtf8().size());
        file.close();
        path="";
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void Widget::on_pushButton_3_clicked()
{
    videoWidget->close();
    delete videoWidget;
    videoWidget=NULL;
    playlist->clear();
}


void Widget::on_horizontalSlider_valueChanged(int value)
{
    play->setVolume(value);
}

void Widget::setAppautoRun(bool flag)
{
    QString app_path=AUTO_PATH;
    if(flag)
    {
        QString app_lnk="./Video.lnk",app_name="./Video.exe";
        QFile::link(app_name,app_lnk);
        QFile::copy(app_lnk,app_path+"\\Video.lnk");
    }

    else QFile::remove(app_path+"\\Video.lnk");
}
