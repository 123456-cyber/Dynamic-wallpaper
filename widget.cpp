#include "widget.h"
#include "ui_widget.h"
#include<QMessageBox>
#include<QCloseEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    play=new QMediaPlayer();
    playlist=new QMediaPlaylist();

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

}

Widget::~Widget()
{
    delete ui;
}

HWND window = NULL;
//遍历窗口句柄
inline BOOL CALLBACK EnumWindowsProc(_In_ HWND tophandle, _In_ LPARAM topparamhandle)
{
    HWND defview = FindWindowEx(tophandle, 0, L"SHELLDLL_DefView", NULL);
    if (defview != NULL)
    {
        window = FindWindowEx(0, tophandle, L"WorkerW", 0);
    }
    return true;
}
//获取底层窗口句柄
HWND GetWorkerW(){
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
        path="";
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::information(this,"提示","关闭该界面后动态壁纸也会随之消失,是否继续？",QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Cancel)
    {
        event->ignore();
    }
    else
    {
        event->accept();
        if(videoWidget!=NULL)
        {
            videoWidget->close();
        }
    }
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
