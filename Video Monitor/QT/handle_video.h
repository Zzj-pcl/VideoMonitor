#ifndef HANDLE_VIDEO_H
#define HANDLE_VIDEO_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QDateTime>
#include <QTextEdit>

using namespace cv;
using namespace std;
class handle_video : public QThread
{
    Q_OBJECT
public:
    explicit handle_video(QObject *parent = 0);

    QTextEdit *textvideo;

signals:
    //void sendData(QString); //用来传递数据的信号

private:
    void run();
    QSettings *Config;
    QString video_path;
    QString img_path;
    QStringList files;
    QStringList nameFilters;
    QDir dir;
    VideoWriter *writer;
    int dir_count;
    int last_dir_cound;
    unsigned long last_rectime;

public slots:

private:

};

#endif // HANDLE_VIDEO_H
