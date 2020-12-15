#ifndef RECV_THREAD_H
#define RECV_THREAD_H
#include <QThread>
#include <QtWidgets/QMainWindow>
#include "postdata.h"
#include <string>
#include <iostream>
#include <recvqueue.h>
#include "recvdata.h"
#include "constdef.h"
#include <iostream>
using namespace std;
//ui 投递消息给发送队列
class RecvThread:public QThread{
    Q_OBJECT
public:
    RecvThread(QObject *parent = nullptr);

    ~RecvThread();
    void StopLoop();

    bool CheckDead();

    void KillThread();

    bool CheckStop();

    void run() override;


private :
    bool m_bStop;
    bool m_bDead;
    mutable std::mutex mu_;
};


#endif
