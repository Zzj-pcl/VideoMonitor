#ifndef CWSKCONTROLWIN_H
#define CWSKCONTROLWIN_H

#include <QWidget>
#include <QGridLayout>
#include <QTimer>
#include <splitscreen.h>

#define WND_MAX_NUM			 64

namespace Ui {
class cwskcontrolwin;
}

class cwskcontrolwin : public QWidget
{
    Q_OBJECT

public:
    explicit cwskcontrolwin(QWidget *parent = 0);
    ~cwskcontrolwin();

    enum ChildState{NO_WND = 0,ONE_WND,TWO_WND,FOUR_WND,EIGHT_WND,SIXTEEN_WND,THIRTY_TWO_WND,SIXTY_FOUR_WND};
    void SetAllShowToFalse();

public slots:
    void UpdateShowWnd();
    void changeWidgetCount(int);

private:
    Ui::cwskcontrolwin *ui;
    int m_nNum;
    std::vector<QGridLayout *> m_vecNLayout;
    std::vector<splitscreen *> playViews;
    splitscreen * getView(uint num);
    void setPlayScreenLayout(ChildState state, int start);
    QTimer m_timer;
    ChildState m_nCurrentState;
    int m_nLayoutStart;

};

#endif // CWSKCONTROLWIN_H
