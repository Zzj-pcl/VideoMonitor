#ifndef DETECTION_H
#define DETECTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include <QThread>

using namespace cv;
using namespace std;

class detection : public QThread
{
public:
    detection();

public:
    void run_face_detec();

    void face_DetectAndDraw(Mat& img,CascadeClassifier& cascade,double scale);

signals:

};

#endif // DETECTION_H
