#ifndef SMART_HOME_H
#define SMART_HOME_H

#include <QMainWindow>

#include <QWidget>
#include <QSound>
#include <QtMultimedia/QMediaPlayer>
#include <QTcpSocket>
#include <QThread>
#include <QMenu>

#include <QProcess>
#include <QMessageBox>
#include <QDateTime>

//video_handle
#include <mainconsole.h>
#include <handle_video.h>

#include <cwskcontrolwin.h>
#include <detection.h>


//  配置
#define Leave_Home 'L'
#define At_Home 'A'

#define CONNECTED 'I'
#define UN_CONNECT 'U'

#define Max_Send_Length 8*1024

#define FIRE_V 29

#define SAVE_FILE 0     // 0|1
#define Debug_Con 0

namespace Ui {
class smart_home;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class EqualizerDialog;

class smart_home : public QWidget
{
    Q_OBJECT
    Ui::smart_home *ui;

public:
    explicit smart_home(QWidget *parent = 0);
    ~smart_home();


protected slots:
    void soltLog();
    void soltConnected();
    void soltRecv();
    void soltClose();
    void clickedAtHome();
    void clickedLeaveHome();
    void checkedBT();
    void btn_videohandle();
    void btn_compose();

    void click_tabwidget_page();
    //void readData(QString);

    void openLocal();
    void openUrl();
    void btn_searchrtsp();
    void btn_searchip();
    void btn_screen();
    void onCompleteCature(QPixmap captureImage);
    void Saveimg();
    void Savevideo();
    void btn_splitfind();
    void btn_Record();
    void onFullScreen();
    void onClip();
    void btn_login();


    void btn_find();
    void btn_num_statistic();
    void btn_face_detec();
    void btn_face_recog();
    void btn_move_detec();
    void btn_people_detec();
    void btn_object_trace();


private:
    inline QString convertSecondToTimeFormat(int second);

private:
    QTcpSocket *socket;
    char house_status;
    char connet_status;
    QSound *s_alarm;
    QSound *s_dingdong;
    QSound *s_enter;
    int temp_h = FIRE_V;    // 温度报警阈值
    QString log;

    bool m_bIsMaximized;
    bool isvideo2=true;
    bool isfacedetect=true;
    QString url;
    QString video_path;
    QString ipv4;
    bool islisten=true;
    int m_clipStartSecond;
    int m_clipEndSecond;
    QLabel *m_pClipLabel;
    QString m_strFileName;

    int len = 0;
    uint pos = 0;
    char pic_data[250 * 1024] = {0};

#if SAVE_FILE
    FILE *cap_pic;
#endif

private:
    VlcInstance *_instance;
    VlcInstance *_instance_2;
    VlcMedia *_media_1;
    VlcMedia *_media_2;
    VlcMediaPlayer *_player;
    VlcMediaPlayer *_player_2;

    EqualizerDialog *_equalizerDialog;

    cwskcontrolwin *_cwskcontrolwin;

    detection* facedetect_thread;

    mainconsole *text_listen;

    //开启服务端监听
    mainconsole *server_listen;
    //开启图片结合视频线程
    handle_video *handle;

};

#endif // SMART_HOME_H
