#include "cwskcontrolwin.h"
#include "ui_cwskcontrolwin.h"
#include <QPainter>

int WndNum[] = {1,2,4,8,16,32,64};

cwskcontrolwin::cwskcontrolwin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cwskcontrolwin)
{
    ui->setupUi(this);
    this->setWindowTitle("多屏监控");

    for (int i = 0; i< WND_MAX_NUM; i++)
       {
           splitscreen *newView = new splitscreen;
           playViews.push_back(newView);
       }
     //  initLayOut();
       m_nCurrentState = NO_WND;

       setPlayScreenLayout(ONE_WND, 0);
       connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeWidgetCount(int)));
       connect(&m_timer,SIGNAL(timeout()),this,SLOT(UpdateShowWnd()));
       m_timer.start(1000);
}

cwskcontrolwin::~cwskcontrolwin()
{
    delete ui;
}

splitscreen *cwskcontrolwin::getView(uint num)
{
//    int nNowWnd = ui->comboBox->currentText().toInt();
    return playViews.at(num);
}

void cwskcontrolwin::setPlayScreenLayout(cwskcontrolwin::ChildState state, int start)
{
    int nNewSize = 0;
    int nOldSize = 0;
    qDebug()<<"state: "<<state;
    if(m_nCurrentState != 0)
    {
        QLayout *oldlayout = ui->frame->layout();
        nOldSize = oldlayout->count();
        qDebug() << "oldSize: " << nOldSize;
        for(int i = 0; i < nOldSize; i++)
        {
            getView(i)->setVisible(false);//使老的布局不可见
        }
        delete oldlayout;
    }

    QGridLayout *newLayout = new QGridLayout();
    switch(state)
    {
        case ONE_WND:
        {
            newLayout->addWidget(getView(0));
            break;
        }
        case TWO_WND:
        {
            newLayout->addWidget(getView(0), 0, 0);
            newLayout->addWidget(getView(1), 1, 0);
            newLayout->setContentsMargins(1, 1, 1, 1);
            newLayout->setSpacing(0);
            break;
        }
        case FOUR_WND:
        {
            newLayout->addWidget(getView(0), 0, 0);
            newLayout->addWidget(getView(1), 0, 1);
            newLayout->addWidget(getView(2), 1, 0);
            newLayout->addWidget(getView(3), 1, 1);
            newLayout->setContentsMargins(1, 1, 1, 1);
            newLayout->setSpacing(0);
            break;
        }
        case EIGHT_WND:
        {
            newLayout->addWidget(getView(0), 0, 0, 3, 3);
            newLayout->addWidget(getView(1), 0, 3);
            newLayout->addWidget(getView(2), 1, 3);
            newLayout->addWidget(getView(3), 2, 3);
            newLayout->addWidget(getView(4), 3, 0);
            newLayout->addWidget(getView(5), 3, 1);
            newLayout->addWidget(getView(6), 3, 2);
            newLayout->addWidget(getView(7), 3, 3);
            newLayout->setContentsMargins(1, 1, 1, 1);
            newLayout->setSpacing(0);
            break;
        }
        case SIXTEEN_WND:
        {
            newLayout->addWidget(getView(0), 0, 0);
            newLayout->addWidget(getView(1), 0, 1);
            newLayout->addWidget(getView(2), 0, 2);
            newLayout->addWidget(getView(3), 0, 3);
            newLayout->addWidget(getView(4), 1, 0);
            newLayout->addWidget(getView(5), 1, 1);
            newLayout->addWidget(getView(6), 1, 2);
            newLayout->addWidget(getView(7), 1, 3);
            newLayout->addWidget(getView(8), 2, 0);
            newLayout->addWidget(getView(9), 2, 1);
            newLayout->addWidget(getView(10), 2, 2);
            newLayout->addWidget(getView(11), 2, 3);
            newLayout->addWidget(getView(12), 3, 0);
            newLayout->addWidget(getView(13), 3, 1);
            newLayout->addWidget(getView(14), 3, 2);
            newLayout->addWidget(getView(15), 3, 3);
            newLayout->setContentsMargins(1, 1, 1, 1);
            newLayout->setSpacing(0);
            break;
        }
        case THIRTY_TWO_WND:
        {
            newLayout->addWidget(getView(0), 0, 0);
            newLayout->addWidget(getView(1), 0, 1);
            newLayout->addWidget(getView(2), 0, 2);
            newLayout->addWidget(getView(3), 0, 3);
            newLayout->addWidget(getView(4), 0, 4);
            newLayout->addWidget(getView(5), 0, 5);
            newLayout->addWidget(getView(6), 0, 6);
            newLayout->addWidget(getView(7), 0, 7);
            newLayout->addWidget(getView(8), 1, 0);
            newLayout->addWidget(getView(9), 1, 1);
            newLayout->addWidget(getView(10), 1, 2);
            newLayout->addWidget(getView(11), 1, 3);
            newLayout->addWidget(getView(12), 1, 4);
            newLayout->addWidget(getView(13), 1, 5);
            newLayout->addWidget(getView(14), 1, 6);
            newLayout->addWidget(getView(15), 1, 7);
            newLayout->addWidget(getView(16), 2, 0);
            newLayout->addWidget(getView(17), 2, 1);
            newLayout->addWidget(getView(18), 2, 2);
            newLayout->addWidget(getView(19), 2, 3);
            newLayout->addWidget(getView(20), 2, 4);
            newLayout->addWidget(getView(21), 2, 5);
            newLayout->addWidget(getView(22), 2, 6);
            newLayout->addWidget(getView(23), 2, 7);
            newLayout->addWidget(getView(24), 3, 0);
            newLayout->addWidget(getView(25), 3, 1);
            newLayout->addWidget(getView(26), 3, 2);
            newLayout->addWidget(getView(27), 3, 3);
            newLayout->addWidget(getView(28), 3, 4);
            newLayout->addWidget(getView(29), 3, 5);
            newLayout->addWidget(getView(30), 3, 6);
            newLayout->addWidget(getView(31), 3, 7);
            newLayout->setContentsMargins(1, 1, 1, 1);
            newLayout->setSpacing(0);
            break;
        }
        default:
        {
            newLayout->addWidget(getView(0), 0, 0);
            newLayout->addWidget(getView(1), 0, 1);
            newLayout->addWidget(getView(2), 0, 2);
            newLayout->addWidget(getView(3), 0, 3);
            newLayout->addWidget(getView(4), 0, 4);
            newLayout->addWidget(getView(5), 0, 5);
            newLayout->addWidget(getView(6), 0, 6);
            newLayout->addWidget(getView(7), 0, 7);
            newLayout->addWidget(getView(8), 1, 0);
            newLayout->addWidget(getView(9), 1, 1);
            newLayout->addWidget(getView(10), 1, 2);
            newLayout->addWidget(getView(11), 1, 3);
            newLayout->addWidget(getView(12), 1, 4);
            newLayout->addWidget(getView(13), 1, 5);
            newLayout->addWidget(getView(14), 1, 6);
            newLayout->addWidget(getView(15), 1, 7);
            newLayout->addWidget(getView(16), 2, 0);
            newLayout->addWidget(getView(17), 2, 1);
            newLayout->addWidget(getView(18), 2, 2);
            newLayout->addWidget(getView(19), 2, 3);
            newLayout->addWidget(getView(20), 2, 4);
            newLayout->addWidget(getView(21), 2, 5);
            newLayout->addWidget(getView(22), 2, 6);
            newLayout->addWidget(getView(23), 2, 7);
            newLayout->addWidget(getView(24), 3, 0);
            newLayout->addWidget(getView(25), 3, 1);
            newLayout->addWidget(getView(26), 3, 2);
            newLayout->addWidget(getView(27), 3, 3);
            newLayout->addWidget(getView(28), 3, 4);
            newLayout->addWidget(getView(29), 3, 5);
            newLayout->addWidget(getView(30), 3, 6);
            newLayout->addWidget(getView(31), 3, 7);
            newLayout->addWidget(getView(32), 4, 0);
            newLayout->addWidget(getView(33), 4, 1);
            newLayout->addWidget(getView(34), 4, 2);
            newLayout->addWidget(getView(35), 4, 3);
            newLayout->addWidget(getView(36), 4, 4);
            newLayout->addWidget(getView(37), 4, 5);
            newLayout->addWidget(getView(38), 4, 6);
            newLayout->addWidget(getView(39), 4, 7);
            newLayout->addWidget(getView(40), 5, 0);
            newLayout->addWidget(getView(41), 5, 1);
            newLayout->addWidget(getView(42), 5, 2);
            newLayout->addWidget(getView(43), 5, 3);
            newLayout->addWidget(getView(44), 5, 4);
            newLayout->addWidget(getView(45), 5, 5);
            newLayout->addWidget(getView(46), 5, 6);
            newLayout->addWidget(getView(47), 5, 7);
            newLayout->addWidget(getView(48), 6, 0);
            newLayout->addWidget(getView(49), 6, 1);
            newLayout->addWidget(getView(50), 6, 2);
            newLayout->addWidget(getView(51), 6, 3);
            newLayout->addWidget(getView(52), 6, 4);
            newLayout->addWidget(getView(53), 6, 5);
            newLayout->addWidget(getView(54), 6, 6);
            newLayout->addWidget(getView(55), 6, 7);
            newLayout->addWidget(getView(56), 7, 0);
            newLayout->addWidget(getView(57), 7, 1);
            newLayout->addWidget(getView(58), 7, 2);
            newLayout->addWidget(getView(59), 7, 3);
            newLayout->addWidget(getView(60), 7, 4);
            newLayout->addWidget(getView(61), 7, 5);
            newLayout->addWidget(getView(62), 7, 6);
            newLayout->addWidget(getView(63), 7, 7);
            newLayout->setContentsMargins(1, 1, 1, 1);
            newLayout->setSpacing(0);
            break;
        }
    }


    for (uint i = 0; i < nNewSize; i++)
    {
        splitscreen * px= getView(i);
        px->setVisible(true);
        qDebug()<<"ControlWin_id: "<<px->winId();
    }

    ui->frame->setLayout(newLayout);
//    m_nLayoutStart = state;
    m_nCurrentState = state;
    m_nNum = nNewSize;

    // 每次都初始化一下
    SetAllShowToFalse();
    UpdateShowWnd();
}

void cwskcontrolwin::SetAllShowToFalse()
{
    int num = ui->comboBox->currentText().toInt();
    for (int i = 0;i < num;++ i)
    {
        playViews.at(i)->SetIsShow(false);
    }
}

void cwskcontrolwin::UpdateShowWnd()
{
    int num = ui->comboBox->currentText().toInt();
    for (int i = 0;i < num;++ i)
    {
        if (!getView(i)->GetIsShow())
        {
            continue;
        }
        for (int j = 0;j < num;++ j)
        {
            if (j != i)
            {
                getView(j)->hide();
            }
        }
        return;
    }
    for (int k = 0;k < num;++ k)
    {
        getView(k)->show();
    }
}

void cwskcontrolwin::changeWidgetCount(int item)
{
    switch(item)
    {
        case 0:{
            setPlayScreenLayout(ONE_WND, 0);
            break;
        }
        case 1:{
            setPlayScreenLayout(TWO_WND, 0);
            break;
        }
        case 2:{
            setPlayScreenLayout(FOUR_WND, 0);
            break;
        }
        case 3:{
            setPlayScreenLayout(EIGHT_WND,0);
            break;
        }
        case 4:{
            setPlayScreenLayout(SIXTEEN_WND,0);
            break;
        }
        case 5:{
            setPlayScreenLayout(THIRTY_TWO_WND,0);
            break;
        }
        default:{
            setPlayScreenLayout(SIXTY_FOUR_WND,0);
            break;
        }
    }
}

