#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include <QObject>
#include <QSettings>
#include <QUdpSocket>
#include <QImage>
#include <QDateTime>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QCoreApplication>
#include <QQueue>
#include <QMessageBox>
#include <QTextEdit>

using namespace std;
using namespace cv;

class mainconsole : public QObject
{
    Q_OBJECT
    //包头
    struct PackageHeader
    {
        //包头大小(sizeof(PackageHeader))
        unsigned int uTransPackageHdrSize;
        //当前包的大小(sizeof(PackageHeader)+数据长度)
        unsigned int uTransPackageSize;
        //数据的总大小
        unsigned int uDataSize;
        //数据被分成包的个数
        unsigned int uDataPackageNum;
        //数据包当前的帧号
        unsigned int uDataPackageCurrIndex;
        //数据包在整个数据中的偏移
        unsigned int uDataPackageOffset;
        //发送时间
        unsigned long SendTime1;
        unsigned long SendTime2;
        //发送帧率
        unsigned int Fps;

    };
    //组包结构体
    struct UdpUnpackData
    {
        char data[1024*1000];
        int length;
    };

public:
    explicit mainconsole(QObject *parent = 0);
    void start_listen(int port);
    QQueue<QByteArray> img_queue;
    QUdpSocket *server;

    QTextEdit *textedit;

    void close_listen();

signals:

private:
    int recvImageNum=0;
    UdpUnpackData imageData;
    Mat img;
    QString Send_time;
    QDateTime time2;
    QSettings *Config;
    QString lose_id;
public slots:
    void ReadPendingDatagrams();

};

#endif // MAINCONSOLE_H
