#include "smart_home.h"
#include <QApplication>

#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>

#include <VLCQtCore/Common.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QCoreApplication::setApplicationName("VLC-Qt Simple Player");
//    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

//    QApplication app(argc, argv);
//    VlcCommon::setPluginPath(app.applicationDirPath() + "/plugins");

    smart_home w;
    w.show();

    return a.exec();
}
