#include "smart_home.h"
#include "ui_smart_home.h"

#include <QtGui>
#include <QPushButton>
#include <QDateTime>
#include <QHostAddress>
#include <QtDebug>
#include <QString>
#include <stdlib.h>
#include <QtNetwork/QTcpSocket>
#include <QPixmap>
#include <QIcon>
#include <QMessageBox>
#include <QTime>
#include <QPixmap>
#include <QFileDialog>
#include <QInputDialog>

#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QHostInfo>
#include <QTextStream>

//vlc
#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>

#include "EqualizerDialog.h"
#include "capturescreen.h"

smart_home::smart_home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::smart_home),
    _media_1(0),
    _media_2(0),
    _equalizerDialog(new EqualizerDialog(this)),
    _cwskcontrolwin(new cwskcontrolwin(this)),
    text_listen(new mainconsole(this)),
    m_bIsMaximized(false),
    m_clipStartSecond(0),
    m_clipEndSecond(0),
    m_pClipLabel(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("物联网视频与监控系统");

    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    // 设置能显示的位数
    ui->HUM_M->setDigitCount(3);
    // 设置显示的模式为十进制
    ui->HUM_M->setMode(QLCDNumber::Dec);
    // 设置显示外观
    ui->HUM_M->setSegmentStyle(QLCDNumber::Flat);
    // 设置样式
    ui->HUM_M->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->TEM_M->setDigitCount(3);
    ui->TEM_M->setMode(QLCDNumber::Dec);
    ui->TEM_M->setSegmentStyle(QLCDNumber::Flat);
    ui->TEM_M->setStyleSheet("border: 1px solid green; color: green; background: silver;");

    ui->IP->setText("192.168.0.100");
    ui->Port->setText("9999");

    ui->Checked->setEnabled(false);
    connect(ui->Checked, SIGNAL(clicked(bool)),
            this, SLOT(checkedBT()) );
    //connect();
    connect(ui->AtHome,SIGNAL(clicked()),
            this,SLOT(clickedAtHome()));

    connect(ui->LeaveHome,SIGNAL(clicked()),
            this,SLOT(clickedLeaveHome()));

    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
    ui->fired->setStyleSheet("color:red");
    ui->fired->setVisible(false);

    ui->record->setReadOnly(true);

    connet_status = UN_CONNECT;

//    m_pClipLabel = new QLabel(this);
//    m_pClipLabel->setText("00:00:00 - 00:00:00");

    ui->comboBox_delaytime1->addItem("100");
    ui->comboBox_delaytime1->addItem("200");
    ui->comboBox_delaytime1->addItem("300");

    ui->comboBox_delaytime2->addItem("100");
    ui->comboBox_delaytime2->addItem("200");
    ui->comboBox_delaytime2->addItem("300");

    ui->label_delay2->setVisible(false);
    ui->comboBox_delaytime2->setVisible(false);
    ui->lineEdit_user->setText("admin");
    ui->lineEdit_pwd->setText("ZALTVP");

    ui->pushButton_facedetec->setEnabled(false);
    ui->pushButton_facerecog->setEnabled(false);
    ui->pushButton_num_statistic->setEnabled(false);

    ui->pushButton_objecttrace->setVisible(false);
    ui->pushButton_peopledetec->setVisible(false);
    ui->pushButton_movedetec->setVisible(false);

    ui->Video->setVisible(false);
    ui->pushButton_Full_Screen->setEnabled(false);

    connect(ui->pushButton_compose,SIGNAL(clicked()),this,SLOT(btn_compose()));
    connect(ui->pushButton_listen,SIGNAL(clicked()),this,SLOT(btn_videohandle()));

    connect(ui->connect, SIGNAL(clicked()),this, SLOT(soltLog()));

    connect(ui->disconnect,SIGNAL(clicked(bool)),this,SLOT(soltClose()));

    s_alarm = new QSound(":/sound/alarm.wav", Q_NULLPTR);
    s_dingdong = new QSound(":/sound/dingdong.wav",Q_NULLPTR);
    s_enter = new QSound(":/sound/enter.wav",Q_NULLPTR);

    //vlc1
    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);
    _equalizerDialog->setMediaPlayer(_player);

    //vlc2
    _instance_2 = new VlcInstance(VlcCommon::args(), this);
    _player_2 = new VlcMediaPlayer(_instance_2);
    _player_2->setVideoWidget(ui->video_2);

    ui->video->setMediaPlayer(_player);
    ui->video_2->setMediaPlayer(_player_2);

    ui->volume->setMediaPlayer(_player);
    ui->volume->setVolume(50);
    ui->seek->setMediaPlayer(_player);


    connect(ui->pushButton__Clip, SIGNAL(clicked()), this, SLOT(onClip()));
    //connect(ui->pushButton_Full_Screen, SIGNAL(triggered()), this, SLOT(onFullScreen()));
    connect(ui->pushButton_localfile, &QPushButton::clicked, this, &smart_home::openLocal);
    connect(ui->pushButton_openrtsp, &QPushButton::clicked, this, &smart_home::openUrl);
    connect(ui->pushButton_pause, &QPushButton::toggled, _player, &VlcMediaPlayer::togglePause);
    connect(ui->pushButton_stop, &QPushButton::clicked, _player, &VlcMediaPlayer::stop);
    connect(ui->equalizer, &QPushButton::clicked, _equalizerDialog, &EqualizerDialog::show);
    connect(ui->pushButton_search_rtsp,SIGNAL(clicked()),this,SLOT(btn_searchrtsp()));
    connect(ui->pushButton_gif,SIGNAL(clicked()),this,SLOT(btn_screen()));
    connect(ui->pushButton_searchip,SIGNAL(clicked()),this,SLOT(btn_searchip()));
    connect(ui->pushButton_saveimg,SIGNAL(clicked()),this,SLOT(Saveimg()));
    connect(ui->pushButton_savevideo,SIGNAL(clicked()),this,SLOT(Savevideo()));
    connect(ui->pushButton_record,SIGNAL(clicked()),this,SLOT(btn_Record()));
    connect(ui->pushButton_connectcam,SIGNAL(clicked()),this,SLOT(btn_login()));


    connect(ui->pushButton_openfind,SIGNAL(clicked()),this,SLOT(btn_find()));
    connect(ui->pushButton_facedetec,SIGNAL(clicked()),this,SLOT(btn_face_detec()));
    connect(ui->pushButton_facerecog,SIGNAL(clicked()),this,SLOT(btn_face_recog()));
    connect(ui->pushButton_movedetec,SIGNAL(clicked()),this,SLOT(btn_move_detec()));
    connect(ui->pushButton_objecttrace,SIGNAL(clicked()),this,SLOT(btn_object_trace()));
    connect(ui->pushButton_peopledetec,SIGNAL(clicked()),this,SLOT(btn_people_detec()));
    connect(ui->pushButton_num_statistic,SIGNAL(clicked()),this,SLOT(btn_num_statistic()));


    //fenpin
    connect(ui->pushButton_splitfind,SIGNAL(clicked()),this,SLOT(btn_splitfind()));

}

smart_home::~smart_home()
{
    //ui->video->release();
    //ui->video_2->release();

    delete _player;
    delete _player_2;
    delete _media_1;
    delete _media_2;
    delete _instance;
    delete _instance_2;

    delete ui;
}
/********************************************************* Init ***************************************************/

void smart_home::clickedAtHome()
{
    house_status = At_Home;
    if(connet_status == UN_CONNECT && !ui->connect->isEnabled())
        ui->connect->setEnabled(true);
    return;
}

void smart_home::clickedLeaveHome()
{
    house_status = Leave_Home;

    if(connet_status == UN_CONNECT && !ui->connect->isEnabled())
        ui->connect->setEnabled(true);

    return;
}

/****************************************************** hose_status ***********************************************/

void smart_home::soltLog()
{
    bool ok;
    int num = ui->Port->text().toInt(&ok,10);
    socket = new QTcpSocket(this);
    connect(this->socket, SIGNAL(connected()),
           this, SLOT(soltConnected()));
   socket->connectToHost(QHostAddress(ui->IP->text()),num);
}

void smart_home::soltConnected()
{
    qDebug()<<"成功连接";//服务器
    ui->connect->setEnabled(false);
    ui->disconnect->setEnabled(true);

    connet_status = CONNECTED;

    connect(socket, SIGNAL(readyRead()),
                this, SLOT(soltRecv()));

}

void smart_home::soltRecv()
{
    char data[Max_Send_Length];
    qint64 ret;
    QString String;
    QStringList ENV;
    QPixmap pixmap;

    while(1)
    {
        if (len > Max_Send_Length || len == 0)
        {
            ret = socket->read(data,Max_Send_Length);
        }
        else
        {
            ret = socket->read(data,len);
        }
        //ret = socket->readLine(data,1500);
        if(-1 == ret || 0 == ret)
            break;
        #if SAVE_FILE
                if( len > 0 && ret != 2 && ret != 6)        //直接保存到文件
                {
                    memcpy( pic_data+pos, data, ret );
                    len -= ret;
                    pos += ret;
/*
                    char buf[20];
                    snprintf(buf, sizeof(buf), "%x %x %x ", data[0]&0xFF, data[1]&0xFF, data[2]&0xFF);
                    qDebug() << "ret.save.PIC: " << ret << "len" << len << " pos:" << pos << "buf:" << buf <<endl;
*/
                    //fwrite(  data, ret, 1, cap_pic );
                    if(len <= 0)
                    {
                        fwrite(  pic_data, pos, 1,cap_pic );
                        fclose(cap_pic);
                        //qDebug()<<"save Pixmap\n";
                        memset(pic_data, 0, sizeof(pic_data));
                        pos = 0;
                        len = 0;
                    }
                    continue;
                }
        #else
                if( len > 0 && ret != 2 && ret != 6)        //直接显示到界面
                {
                    memcpy( pic_data+pos, data, ret );
                    len -= ret;
                    pos += ret;
                    //qDebug() << "ret.PIC: " << ret << "len" << len << " pos:" << pos <<"\r\n";
                    if(len <= 0)
                    {
                        QPixmap *pix = new QPixmap(320,240);
                        pix->loadFromData( (uchar*)pic_data, pos, "JPEG" );
                        ui->Video->setPixmap( *pix );
                        //qDebug()<<"set Pixmap\n";
                        memset(pic_data, 0, sizeof(pic_data));
                        pos = 0;
                        len = 0;
                    }
                    continue;
                }
        #endif
        switch(data[0])
        {
            case 'E'://温湿度采集
            //qDebug() << "error data:" << data << "\r\n";
                for(int i = 0;i < 5;i++)
                {
                    String[i] = QChar(data[i+1]);
                }
                qDebug() << String;
                ENV = String.split(',');

                ui->HUM_M->display(ENV[0].toInt());
                ui->TEM_M->display(ENV[1].toInt());
                if( ENV[1].toInt() > temp_h )
                {
                    if( s_alarm->isFinished() )
                    {
                        s_alarm->play();
                    }
                    ui->fired->setVisible(true);
                }else{
                    s_alarm->stop();
                    ui->fired->setVisible(false);
                }
            break;
            case 'I'://红外检测
            //qDebug() << "error data:" << data << "\r\n";
                if('1' == data[1])
                {
                    ui->Checked->setEnabled(true);
                    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
                    if(house_status == At_Home)
                    {
                        log.append("You have a visitor , " + time.toString("yyyy-MM-dd hh:mm:ss") + "\r\n");
                        ui->record->setText(log);
                        if( !pixmap.load(":/jpg/green.png",Q_NULLPTR,Qt::AutoColor) )
                            qDebug() << "load green.png error\n";
                        if( s_dingdong->isFinished() )
                        {
                            s_dingdong->setLoops(3);
                            s_dingdong->play();
                        }
                    }
                    if(house_status == Leave_Home)
                    {
                        log.append("Somebody broke in, " + time.toString("yyyy-MM-dd hh:mm:ss") + "\r\n");
                        ui->record->setText(log);
                        if( !pixmap.load(":/jpg/red.png",Q_NULLPTR,Qt::AutoColor) )
                            qDebug() << "load green.png error\n";
                        if( s_enter->isFinished() )
                        {
                            s_enter->setLoops(100);
                            s_enter->play();
                        }
                    }
                    ui->Light->setPixmap(pixmap);
                }
            break;
            case 'H'://Header
            {
                QString *length = new QString( data+1 );
                len = length->toUInt(Q_NULLPTR,10);
                pos = 0;
                memset( pic_data, 0 ,sizeof(pic_data));
                //qDebug()<<"ret.H:"<< ret <<" ,Header_len:"<< len <<"\n";
                if(ret > 10)
                {
                    ret -= 10;
                    memcpy( pic_data+pos, data+10, ret );
                    len -= ret;
                    pos += ret;
                    char buf[20];
                    snprintf(buf, sizeof(buf), "%x %x %x ", data[10]&0xFF, data[11]&0xFF, data[12]&0xFF);
                    //qDebug() << "ret.save.PIC: " << ret << "len" << len << " pos:" << pos << "buf:" << buf <<endl;
                }
        #if SAVE_FILE
                cap_pic = fopen("cap_picture.jpg", "wb+");
                if(!cap_pic) //读取失败 if(!fp) 与 if(fp == NULL) 等价
                {
                    qDebug()<< "fopen err";
                    return;
                }
        #endif
            }
            break;
            default :
            break;
        }
    }
}

void smart_home::checkedBT()
{
    if( !s_dingdong->isFinished() )
        s_dingdong->stop();
    if( !s_enter->isFinished() )
        s_enter->stop();
    ui->Light->clear();
    ui->Checked->setEnabled(false);
}

void smart_home::soltClose()
{
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
    socket->close();
    connet_status = UN_CONNECT;
    ui->HUM_M->display(0);
    ui->TEM_M->display(0);
    len = 0;
    log.clear();
    ui->record->clear();
    ui->Light->clear();
    ui->fired->setVisible(false);
    if( !s_alarm->isFinished() )
        s_alarm->stop();
    if( !s_dingdong->isFinished() )
        s_dingdong->stop();
    if( !s_enter->isFinished() )
        s_enter->stop();

#if SAVE_FILE
    if( cap_pic != NULL)
        fclose(cap_pic);
#endif
}

void smart_home::click_tabwidget_page()
{
//    if()
//        text_listen->textedit->close();
}

void smart_home::btn_videohandle()
{
    //开启服务端监听
    server_listen =new mainconsole();
    //开启图片结合视频线程
    handle = new handle_video();

    if(islisten==true)
    {

        QSettings *ConfigWriter = new QSettings(QCoreApplication::applicationDirPath()+"/Config.ini",
                                                QSettings::IniFormat);
        if (ConfigWriter->value("/Path/Img/")==QVariant::Invalid
                ||ConfigWriter->value("/Path/Video/")==QVariant::Invalid
                ||ConfigWriter->value("/Listen/port/")==QVariant::Invalid){
            //当配置文件为空或不完整时,重新初始化文件
            QMessageBox::information(NULL, "warning", "!!!!!!!配置信息不完整,初始化配置信息!!!!!!!!!!!", QMessageBox::Yes, QMessageBox::Yes);
            ConfigWriter->setValue("/Path/Img/",QCoreApplication::applicationDirPath()+"/img/");
            ConfigWriter->setValue("/Path/Video/",QCoreApplication::applicationDirPath()+"/video/");
            ConfigWriter->setValue("/Listen/port/",5354);
            return;
        }
        delete ConfigWriter;

        QSettings *Config = new QSettings(QCoreApplication::applicationDirPath()+"/Config.ini",
                                                QSettings::IniFormat);

        QString imgip = QString::fromStdString(Config->value("/Path/Img/").toString().toStdString());
        ui->lineEdit_photosaveip->setText(imgip);

        QString videoip = QString::fromStdString(Config->value("/Path/Video/").toString().toStdString());
        ui->lineEdit_moviesaveip->setText(videoip);

        QString listenport = QString::fromStdString(Config->value("/Listen/port/").toString().toStdString());
        ui->lineEdit_listenport->setText(listenport);

        //清空图片文件夹
        QDir dir(Config->value("/Path/Img/").toString());
        for(int i=0;i<dir.count();i++)
        {
            dir.remove(dir[i]);
        }
        //获取主机名
        QString localHostName = QHostInfo::localHostName();
        //获取当前Ip地址
        QHostInfo info = QHostInfo::fromName(localHostName);
        //只输出IPv4地址（可能有多条
        foreach(QHostAddress address,info.addresses())
        {
            if(address.protocol() == QAbstractSocket::IPv4Protocol)
                ipv4 = QString::fromStdString(address.toString().toStdString());
                ui->lineEdit_ipv4->setText(ipv4);
        }

        ui->pushButton_listen->setText("停止");
        islisten=false;

        int port = ConfigWriter->value("/Listen/port/").toInt();
        server_listen->start_listen(port);

        handle->start();
    }
    else
     if(islisten==false)
     {

        server_listen->disconnect();
        server_listen->server->disconnect();    
        server_listen->server->close();
        delete server_listen;

        handle->textvideo->close();
        //quit handle QThread     
        handle->quit();
        handle->wait();
        delete handle;


        ui->pushButton_listen->setText("监听合成视频");
        islisten=true;

        ui->lineEdit_ipv4->clear();
        ui->lineEdit_listenport->clear();
        ui->lineEdit_moviesaveip->clear();
        ui->lineEdit_photosaveip->clear();
    }

}

void smart_home::btn_compose()
{

}

void smart_home::onFullScreen()
{
//    if(isFullScreen())
//    {
//        ui->video->showFullScreen();
//        ui->video->showMaximized();
//        m_bIsMaximized ? showMaximized() : showNormal();
//    }
//    else
//    {
//        ui->video->showFullScreen();
//        ui->video->showMaximized();
//        m_bIsMaximized = isMaximized();
//    }
}

void smart_home::openLocal()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));
    if (file.isEmpty())
        return;

    _media_1 = new VlcMedia(file, true, _instance);

    _player->open(_media_1);
}

void smart_home::openUrl()
{
    url=ui->lineEdit_rtsp->text();
    if (url.isEmpty())
        return;

    _media_1 = new VlcMedia(url, _instance);

    _player->open(_media_1);

}

void smart_home::btn_searchip()
{
    QString cameraip="192.168.0.111";
    QString port="8000";
    ui->lineEdit_IP->setText(cameraip);
    ui->lineEdit_port->setText(port);
}

void smart_home::btn_searchrtsp()
{
    string cameraip="192.168.0.111";
    QString rtsp=QString::fromStdString("rtsp://admin:ZALTVP@"+cameraip+":554/mpeg4/ch1/main/av_stream"); //+"--newwork-caching=200"
    ui->lineEdit_rtsp->setText(rtsp);
}

void smart_home::btn_screen()
{
   //点击截图按钮开始截图;
   capturescreen* captureHelper = new capturescreen();
   connect(captureHelper, SIGNAL(signalCompleteCature(QPixmap)), this, SLOT(onCompleteCature(QPixmap)));
   captureHelper->show();

}

void smart_home::onCompleteCature(QPixmap captureImage)
{

    ui->label_img->setPixmap(captureImage);
}

//截图存为文件
void smart_home::Saveimg()
{
    //默认保存路径为当前应用程序路径下的Screen文件夹
    QString strDir = QCoreApplication::applicationDirPath() + "\\Screen";
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkdir(strDir);
    }

    QString strFile = strDir + "\\数据快照" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + ".png";
    QPixmap pix = QPixmap::grabWidget(ui->video);

    QString fileName = QFileDialog::getSaveFileName(this,"保存图片",strFile,"PNG (*.png);;BMP (*.bmp);;JPEG (*.jpg *.jpeg)");
    if (!fileName.isNull())
    {
        pix.save(fileName);
        return;
    }
}

void smart_home::btn_splitfind()
{
    cwskcontrolwin* cw=new cwskcontrolwin();
    cw->show();
}

void smart_home::Savevideo()
{
    video_path= QFileDialog::getExistingDirectory(
                this, "choose src Directory",
                 "/");;
    video_path = "/home/riki/Desktop/RQ_HMS/build-smart_home-Desktop-Debug";
    try
    {
        QString videoaddress=QString::fromStdString(video_path.toStdString());
        ui->lineEdit_savevideo->setText(videoaddress);

    }
    catch(Exception)
    {

    }

}

QString smart_home::convertSecondToTimeFormat(int second)
{
    return QTime(0, 0, 0).addSecs(second).toString("HH:mm:ss");
}

void smart_home::onClip()
{
    QString strStartTime = convertSecondToTimeFormat(m_clipStartSecond);
    QString strEndTime = convertSecondToTimeFormat(m_clipEndSecond);

    QMessageBox box(this);
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setText(tr("[%1, %2]").arg(strStartTime).arg(strEndTime));
    if(box.exec() == QMessageBox::Ok)
    {
        QString strOutName = QFileInfo(m_strFileName).absoluteDir().absolutePath() + "/out." + QFileInfo(m_strFileName).suffix();
        QString cmd = QString("ffmpeg -i %1 -vcodec copy -acodec copy -ss %2 -to %3 %4 -y").arg(m_strFileName).arg(strStartTime).arg(strEndTime).arg(strOutName);
        qDebug() << "====" << cmd;

        QMessageBox testMassage;
        testMassage.setText(cmd);
        testMassage.show();
        qApp->processEvents();

        QProcess p;
        p.start("cmd", QStringList() << "/c" << cmd);
        p.waitForStarted();
        p.waitForFinished();
        //QString strTemp=QString::fromLocal8Bit(p.readAllStandardOutput());
    }
}

void smart_home::btn_Record()
{
    url=ui->lineEdit_rtsp->text();
    if(url.isEmpty())
       return;
    cv::VideoCapture cap(url.toStdString());
    if(!cap.isOpened())
    {
      qDebug()<< "open camera failed. ";
      return;
    }

    Mat Frame;
    //设置保存的视频帧数目
    int frameNum = ui->lineEdit_framenum->text().toInt();
    if(frameNum==NULL)
        qDebug()<<"error";
        return;
    //保存视频的路径
    string outputVideoPath = video_path.toStdString() + "record.avi";
    //获取当前摄像头的视频信息
    cv::Size sWH = cv::Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
        (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    VideoWriter outputVideo;
    outputVideo.open(outputVideoPath, CV_FOURCC('M', 'P', '4', '2'), 25.0, sWH);

    while (cap.isOpened() && frameNum > 0)
    {
        cap >> Frame;
        if (Frame.empty()) break;
        outputVideo << Frame;
        frameNum--;

        imshow("img", Frame);
        waitKey(10);

        if (char(waitKey(1)) == 'q')
            break;
    }
    outputVideo.release();
    QMessageBox::information(NULL, "tip", "Finish", QMessageBox::Ok,QMessageBox::Ok);

}

void smart_home::btn_find()
{
    if(isvideo2==true)
    {
        QString url_find =
               QInputDialog::getText(this, tr("Open RTSP"), tr("RTSP address"));

       if (url_find.isEmpty())
           return;

       _media_2 = new VlcMedia(url_find, _instance_2);

       _player_2->open(_media_2);

       ui->pushButton_openfind->setText("关闭监控");
       isvideo2=false;

       ui->pushButton_facedetec->setEnabled(true);
       ui->pushButton_facerecog->setEnabled(true);
       ui->pushButton_num_statistic->setEnabled(true);
       ui->pushButton_openrtsp->setEnabled(false);
    }

   else if(isvideo2==false)
     {
        _player_2->stop();
        ui->pushButton_openfind->setText("打开监控");
        isvideo2=true;
        ui->pushButton_facedetec->setEnabled(false);
        ui->pushButton_facerecog->setEnabled(false);
        ui->pushButton_num_statistic->setEnabled(false);
        ui->pushButton_openrtsp->setEnabled(true);
     }
}

bool isconcam=true;
void smart_home::btn_login()
{
    if(ui->lineEdit_user->text().isEmpty()
            ||ui->lineEdit_pwd->text().isEmpty())
        QMessageBox::information(NULL, "error", "用户名或密码不能为空!", QMessageBox::Yes, QMessageBox::Yes);
    if(ui->lineEdit_user->text()!="admin"
                ||ui->lineEdit_pwd->text()!="ZALTVP")
            QMessageBox::information(NULL, "error", "用户名或密码错误!", QMessageBox::Yes, QMessageBox::Yes);
    if(ui->lineEdit_user->text()=="admin"&&ui->lineEdit_pwd->text()=="ZALTVP")
            QMessageBox::information(NULL, "OK", "success", QMessageBox::Yes, QMessageBox::Yes);
            ui->pushButton_connectcam->setEnabled(false);
            isconcam=false;
}


void smart_home::btn_face_detec()
{
    url=ui->lineEdit_rtsp->text();
    cv::VideoCapture capture(url.toStdString());
    if(!capture.isOpened())
    {
      qDebug()<< "open camera failed. ";
      return;
    }
    qDebug()<<"open camera succeed. ";

    ui->pushButton_facedetec->setText("关闭");
    CascadeClassifier faceCascade;
    double scale = 4;
    int		nRet = 0;
    /* 加载分类器 */
    nRet = faceCascade.load("/home/riki/opencv-2.4.13/data/haarcascades/haarcascade_frontalface_alt.xml");

    if(!nRet)
    {
       qDebug()<<"load xml failed.\n";
       return;
    }

    Mat frame;
    while(1)
    {
        capture >> frame;
        if(frame.empty())
        {
            continue;
        }
        Mat frame1 = frame.clone();
        detection* facedete=new detection;
        facedete->face_DetectAndDraw( frame1, faceCascade, scale );
        if(waitKey(1) > 0)		// delay ms 等待按键退出
        {
            break;
        }
    }
    ui->pushButton_facedetec->setText("人脸检测");
}

void smart_home::btn_face_recog()
{

}


string int2str(const int &int_temp)//数据流之间的转换
{
    stringstream stream;//通过流来实现字符串和数字的转换
    string string_temp;//有不限长度的优点
    stream << int_temp; //将int输入流，向左移4位
    string_temp = stream.str();  //此处也可以用 stream>>string_temp//从stream中抽取前面插入的int值

    return string_temp;
}

void smart_home::btn_num_statistic()
{
    url=ui->lineEdit_rtsp->text();
    VideoCapture cap(url.toStdString());

    Mat img, src, frame, frame_gray, first_frame, threshold_src, gass_src, dilate_src;
    int num=0;
    ui->pushButton_num_statistic->setText("关闭");
    while (1)
    {
        cap >> first_frame;
        cvtColor(first_frame, first_frame, CV_BGR2GRAY);

        cap >> frame;
        if (frame.empty())
            break;
        vector<Vec4i> hierarchy;
        vector<vector<Point> > contour;
        cvtColor(frame, frame_gray, CV_BGR2GRAY);
        absdiff(frame_gray,first_frame,src);
        threshold(src, threshold_src,50,255,THRESH_BINARY);
        GaussianBlur(threshold_src, gass_src,Size(5,5),1.5);

        dilate(gass_src, dilate_src,Mat());
        findContours(dilate_src,contour, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < contour.size();i++)
        {
            Rect bndRect = boundingRect(contour.at(i));
            Point p1,p2;
            p1.x = bndRect.x;
            p1.y = bndRect.y;
            p2.x = bndRect.x + bndRect.width;
            p2.y = bndRect.y + bndRect.height;
            if (bndRect.area()>3000){
                rectangle(frame, p1, p2, Scalar(0, 0, 255));
                num++;
            }
        }
        string font = "                                          current number:";
        putText(frame,font+int2str(num),Point(100,50),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,0,255));

        num = 0;
        imshow("frame", frame);
        //imshow("dilate_src", dilate_src);
        //imshow("first_frame", first_frame);
        if(waitKey(1) > 0)		// delay ms 等待按键退出
        {
            break;
        }
    }
    ui->pushButton_num_statistic->setText("人数统计");
}

void smart_home::btn_people_detec()
{

}

void smart_home::btn_object_trace()
{

}

void smart_home::btn_move_detec()
{

}
