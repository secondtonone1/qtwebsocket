#include "refreshpkg.h"
#include "logFile.h"
#include <QApplication>
#include <QDebug>
#include "devdata.h"
int main(int argc, char *argv[])
{
    //注册MessageHandler
    //qInstallMessageHandler(outputMessage); //注册MessageHandler

    //暂时屏蔽以便调试信息输出到屏幕，方便调试
    logInit();


    QApplication a(argc, argv);
    //注册元类型

    qRegisterMetaType<DevData>("DevData");
    qRegisterMetaType<DevData>("DevData&");

    RefreshPkg w;
    w.show();
    return a.exec();
}
