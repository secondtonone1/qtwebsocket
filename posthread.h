#ifndef POST_THREAD_H
#define POST_THREAD_H
#include <QThread>
#include <QtWidgets/QMainWindow>
#include "postdata.h"
#include <string>
#include <iostream>
#include <recvqueue.h>
#include "recvdata.h"
#include "constdef.h"
#include <iostream>
class ServerThread;
using namespace std;
//ui 投递消息给发送队列
class PostThread:public QThread{
    Q_OBJECT
public:
    PostThread(QObject *parent = nullptr, ServerThread * srvth = nullptr);

    ~PostThread();
    void StopLoop();

    bool CheckDead();

    void KillThread();

    bool CheckStop();

    void run() override;


private :
    bool m_bStop;
    bool m_bDead;
    mutable std::mutex mu_;
    ServerThread * m_pServerThread;
};


#endif
