#ifndef SPLITSCREEN_H
#define SPLITSCREEN_H

#include <QWidget>
#include <QFrame>
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>

class splitscreen : public QFrame
{
    Q_OBJECT
public:
    explicit splitscreen(QWidget *parent = 0);

    bool GetIsShow(){return m_bShow;}
    void SetIsShow(bool bShow = false){ m_bShow = bShow;}

protected:
    void mouseDoubleClickEvent(QMouseEvent *);

signals:


public slots:

private:
    bool m_bShow;

};

#endif // SPLITSCREEN_H
