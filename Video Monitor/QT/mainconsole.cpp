#include "mainconsole.h"


mainconsole::mainconsole(QObject *parent) : QObject(parent)
{
  server=new QUdpSocket();

  textedit=new QTextEdit();
  //textedit->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
  textedit->setWindowTitle("listen_data");
  textedit->setEnabled(false);
  textedit->setGeometry(20,280,801,281);
}

void mainconsole::close_listen()
{

}

void mainconsole::start_listen(int port)
{
    bool ok = server->bind(QHostAddress::Any,port);
    if(!ok)
    {
      QMessageBox::information(NULL, "error", "!!!!!!您绑定的服务端口被占用,请更换Config.ini下设置的端口!!!!!!", QMessageBox::Ok,QMessageBox::Ok);
      return;
    }
    //监听消息的传入
    connect(server,SIGNAL(readyRead()),this,SLOT(ReadPendingDatagrams()));
    textedit->show();
}

void mainconsole::ReadPendingDatagrams(){
    QByteArray datagram;
   while (server->hasPendingDatagrams())
   {
          datagram.resize(server->pendingDatagramSize());
          QHostAddress sender;
          quint16 senderPort;
          server->readDatagram(
                      datagram.data(),
                      datagram.size(),
                      &sender,
                      &senderPort);

          //组包开始+++++++++++++++++++++++++++
          static int num = 1;
          static uint size = 0;

          PackageHeader *packageHead = (PackageHeader *)datagram.data();
          QString aa=QString::fromStdString("*************************************************************");      
          textedit->append(aa);
          //qDebug()<<aa;

          QString bb=QString::fromStdString("来自"+sender.toString().toStdString()+":")+QString::number(senderPort);        
          textedit->append(bb);
          //qDebug()<<bb;

          QString Send_time2 = QString::number(packageHead->SendTime1)+QString::number(packageHead->SendTime2);
          QString cc=QString::fromStdString("当前帧号:"+Send_time2.toStdString());       
          textedit->append(cc);
          //qDebug()<<cc;

          QString dd=QString::fromStdString("当前包号:"+QString::number(packageHead->uDataPackageCurrIndex).toStdString());
          textedit->append(dd);
          //qDebug()<<dd;

          QString ff=QString::fromStdString("包的总个数:"+QString::number(packageHead->uDataPackageNum).toStdString());
          textedit->append(ff);
          //qDebug()<<ff;

          //cout<<"*************************************************************"<<endl;
          //cout<<"来自"<<sender.toString().toStdString()<<":"<<senderPort<<endl;
          //QString Send_time2 = QString::number(packageHead->SendTime1)+QString::number(packageHead->SendTime2);
          //cout<<"当前帧号:"<<Send_time2.toStdString()<<endl;
          //cout<<"当前包号:"<<packageHead->uDataPackageCurrIndex<<endl;
          //cout<<"包的总个数:"<<packageHead->uDataPackageNum<<endl;
          if (packageHead->uDataPackageCurrIndex == num)
          {    //不丢包的情况
              //当num等于当前的包号进入循环 否则不进入
              //原理: 因为第一个发过来的包号为0 和num的0进行比较,num再进行++ 一旦丢失某个包,当前包号就不会和num相等
              //将lose_id 归0
              lose_id='0';
              num++;
              size += packageHead->uTransPackageSize-packageHead->uTransPackageHdrSize;
              if (size > 1024*1000)
              {
                  qDebug()<<"image too big";
                  num = 1;
                  size = 0;
                  return;
              }
              if (packageHead->uDataPackageOffset > 1024*1000)
              {
                  qDebug()<<"image too big";
                  packageHead->uDataPackageOffset = 0;
                  num = 1;
                  size = 0;
                  return;
              }

              memcpy(imageData.data+packageHead->uDataPackageOffset, datagram.data()+packageHead->uTransPackageHdrSize,
                     packageHead->uTransPackageSize-packageHead->uTransPackageHdrSize);
              if ((packageHead->uDataPackageNum == packageHead->uDataPackageCurrIndex)
                      && (size == packageHead->uDataSize))
              {
                  //当包的完整性检验成功
                  imageData.length = packageHead->uDataSize;

                  QImage image;
                  image.loadFromData((uchar *)imageData.data,imageData.length,"jpg");
                  img = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
                  QString time_int = QString::number(packageHead->SendTime1)+QString::number(packageHead->SendTime2);
                  QString year = time_int.mid(0,10);
                  QString mecs = time_int.mid(9,3);
                  time2 = QDateTime::fromTime_t(year.toLong());
                  time2 = time2.addMSecs(mecs.toInt());
                  Send_time=time2.toString("yyyy-MM-dd hh:mm:ss:zzz");

                  //cout<<Send_time.toStdString()<<"接收到一张图片"<<endl;
                  QString ee=QString::fromStdString(Send_time.toStdString()+"接收到一张图片");
                  textedit->append(ee);
                  //qDebug()<<ee;

                  //获取fps
                  string FPS = QString::number(packageHead->Fps,10).toStdString();
                  //保存图片到相对应的目录中

                  Config=new QSettings(QCoreApplication::applicationDirPath()+"/Config.ini",QSettings::IniFormat);
                  QString gg=QString::fromStdString("保存的位置及名称:"+Config->value("/Path/Img/").toString().toStdString()+year.toStdString()+"#"+FPS+"#"+Send_time.toStdString()+"#.jpg");
                  textedit->append(gg);
                  //qDebug()<<gg;

                  //cout<<"保存的位置及名称:"<<Config->value("/Path/Img/").toString().toStdString()+year.toStdString()+"#"+FPS+"#"+Send_time.toStdString()+"#.jpg"<<endl;
                  imwrite(Config->value("/Path/Img/").toString().toStdString()+year.toStdString()+"#"+FPS+"#"+Send_time.toStdString()+"#.jpg",img);
                  imwrite(Config->value("/Path/Img/").toString().toStdString()+"index.jpg",img);



                  //归0
                  memset(&imageData,0,sizeof(UdpUnpackData));

                  num = 1;
                  size = 0;
              }
          }
          else
          {
              //发生丢包情况的处理
              num = 1;
              size = 0;
              memset(&imageData,0,sizeof(UdpUnpackData));
              lose_id=Send_time2;
              QString hh=QString::fromStdString("!!!!!!!!!!!!当前图片帧:"+lose_id.toStdString()+"存在丢包情况!!!!!!!!!!");
              textedit->append(hh);
              //qDebug()<<hh;

          }

          //每次处理 判断是否有丢包情况发生 有的话当下一图片帧发送过来时发送丢包重发报文
          if(lose_id!="0"){
              if(Send_time2!=lose_id){
                  //写重发报文
                  QString Resend = "RESEND#"+lose_id;
                  server->writeDatagram(
                           Resend.toUtf8(), Resend.size(),
                           sender, senderPort);
            //cout<<"!!!!!!!!!!!!帧号:"<<lose_id.toStdString()<<"存在丢包情况,已发送重发消息!!!!!!!!!!"<<endl;
            QString jj=QString::fromStdString("!!!!!!!!!!!!帧号:"+lose_id.toStdString()+"存在丢包情况,已发送重发消息!!!!!!!!!!");
            textedit->append(jj);
            //qDebug()<<jj;

              }
          }
          //cout<<"-------------------------------------------------------------"<<endl;
          QString kk=QString::fromStdString("-------------------------------------------------------------");
          textedit->append(kk);
          //qDebug()<<kk;
          //组包结束+++++++++++++++++++++++++++
   }
}

