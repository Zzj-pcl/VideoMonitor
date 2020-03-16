#include "detection.h"

#include <QDebug>

detection::detection()
{

}

void detection::run_face_detec()
{

}

void detection::face_DetectAndDraw(Mat& img,CascadeClassifier& cascade,double scale)
{
    double t = 0;
    vector<Rect> faces;
    cv::Mat gray, smallImg;
    double fx = 1 / scale;

    cvtColor( img, gray, COLOR_BGR2GRAY );	// 将源图像转为灰度图

    /* 缩放图像 */
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );	// 直方图均衡化，提高图像质量

    /* 检测目标 */
    t = (double)getTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );
//    t = (double)getTickCount() - t;
//    printf( "detection time = %g ms\n", t*1000/getTickFrequency());

    /* 画矩形框出目标 */
    for ( size_t i = 0; i < faces.size(); i++ ) // faces.size():检测到的目标数量
    {
        Rect rectFace = faces[i];

        rectangle(	img, Point(rectFace.x, rectFace.y) * scale,
                    Point(rectFace.x + rectFace.width, rectFace.y + rectFace.height) * scale,
                    Scalar(0, 255, 0), 2, 8);
    }
    imshow("FaceDetect", img);
}

