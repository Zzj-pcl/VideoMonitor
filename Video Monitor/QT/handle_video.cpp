#include "handle_video.h"

handle_video::handle_video(QObject *parent) : QThread(parent)
{
    textvideo=new QTextEdit();
    textvideo->setWindowTitle("video_data");
    textvideo->setEnabled(false);
    textvideo->setGeometry(100,480,500,100);
    textvideo->show();

    Config = new QSettings(QCoreApplication::applicationDirPath()+"/Config.ini",QSettings::IniFormat);
    video_path = Config->value("/Path/Video/").toString();
    img_path = Config->value("/Path/Img/").toString();
}

void handle_video::run()
{
   unsigned long last_time=0;
   int sumFps=0;
   last_dir_cound=0;
   while(1)
   {
       //获取文件夹下所有的文件
       dir.setPath(img_path);
       //设置读取的文件的格式
       nameFilters << "*.jpg";
       dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
       dir.setNameFilters(nameFilters);
        //统计jpg格式的文件个数
       dir_count = dir.count();
       //当文件下的文件数量大于1 开始处理文件
       while(dir_count>1)
       {
                 Mat img;
                //获取文件夹下所有的文件
                dir.setPath(img_path);
                //设置读取的文件的格式
                nameFilters << "*.jpg";
                dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
                dir.setNameFilters(nameFilters);
                 //统计jpg格式的文件个数
                     dir_count = dir.count();
                     QStringList img_list;
                     QStringList video_img_list;

                     if(last_dir_cound!=dir_count)
                     {
                         last_dir_cound=dir_count;

                         last_rectime=QDateTime::currentDateTime().toTime_t();
                     }
                     //获取所有的文件的list
                     for(int i=0;i<dir_count;i++)
                     {
                         img_list.append(dir[i]);
                     }

                     //排序
                     img_list.sort();
                //判断文件夹是否为空
                //处理每个文件
                int rows=0;
                if(dir_count!=0){
                    //遍历文件夹下的所有文件
                    for(int i=0;i<dir_count;i++)
                    {
                        //处理非index文件
                        if(img_list[i]!=QString("index.jpg"))
                        {

                            QStringList name_split = img_list[i].split("#");
                            unsigned long s =name_split[0].toLong();
                            int f =name_split[1].toInt();
                            QString time=name_split[2];
                            //qDebug()<<QDateTime::currentDateTime().toTime_t()-last_rectime;
                            //当两个视频帧相差60秒 或者一分钟内未收到帧
                            if((last_time+60)<s||QDateTime::currentDateTime().toTime_t()-last_rectime>=60){
                                //当没有出现新的时间段时
                                //判断待合成视频列表是否为空
                                //为空说明为第一次
                                //不为空遍历合成
                                if(!video_img_list.empty()){

                                    try
                                    {

                                     string time=QDateTime::fromTime_t(s).toString("yyyy-MM-dd hh:mm:ss:zzz").toStdString();
                                     //cout<<"视频保存地址:"<<video_path.toStdString()+time+".mp4";
                                     QString videosave=QString::fromStdString("视频保存地址:"+video_path.toStdString()+time+".mp4");
                                     qDebug()<<videosave;
                                     textvideo->append(videosave);
                                     //ui->textEdit->append(videosave);

                                     img = imread(img_path.toStdString()+video_img_list[0].toStdString());
                                    if (video_img_list.size()<5)
                                    {
                                        video_img_list.clear();
                                    }
                                    writer = new VideoWriter(video_path.toStdString()+time+".mp4",
                                      CV_FOURCC('m', 'p', '4', 'v'),25,
                                      Size(img.cols,img.rows));

                                    for(int i =0;i<video_img_list.size();i++)
                                    {
                                        img = imread(img_path.toStdString()+video_img_list[i].toStdString());


                                        writer->write(img);
                                        //删除文件
                                        dir.remove(dir[i]);
                                    }
                                    }
                                    catch(Exception)
                                    {

                                    }

                                    writer->release();
                                    video_img_list.clear();
                                }
                                else
                                {
                                    //为空情况 添加
                                  qDebug()<<"为空情况";
                                  video_img_list.append(dir[i]);
                                }

                                last_time=s;

                            }
                            else
                            {
                                video_img_list.append(dir[i]);
                            }


                        }
                    }
                }

            }


            QThread::msleep(300);
    }

}
