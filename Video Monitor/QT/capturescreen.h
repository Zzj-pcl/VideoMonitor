#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H

#include <QWidget>
#include <QPainter>
#include <QMessageBox>

class capturescreen : public QWidget
{
    Q_OBJECT
public:
    explicit capturescreen(QWidget *parent = 0);

signals:

Q_SIGNALS:
    void signalCompleteCature(QPixmap catureImage);

public slots:

private:
    void initWindow();
    void loadBackgroundPixmap();

    QRect getRect(const QPoint &beginPoint, const QPoint &endPoint);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool m_isMousePress;
    QPixmap m_loadPixmap, m_capturePixmap;
    int m_screenwidth;
    int m_screenheight;
    QPoint m_beginPoint, m_endPoint;
    QPainter m_painter;

};

#endif // CAPTURESCREEN_H
