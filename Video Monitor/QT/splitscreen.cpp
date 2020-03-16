#include "splitscreen.h"
#include <QPainter>

splitscreen::splitscreen(QWidget *parent)  : QFrame(parent)
{
    m_bShow = false;
    this->setStyleSheet("QFrame{ border: 1px solid #0000CD;}");//蓝色
}

void splitscreen::mouseDoubleClickEvent(QMouseEvent *)
{
    qDebug() << "is double click ... " << this->winId();
    m_bShow = !m_bShow;
}


