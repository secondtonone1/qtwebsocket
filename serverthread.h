#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H
#include <QThread>
#include <QtWidgets/QMainWindow>
#include "postdata.h"
#include <string>
#include <iostream>
#include <postqueue.h>
#include "server.h"
using namespace std;
class ServerThread:public QThread{
    Q_OBJECT
public:
    ServerThread(quint16 port, bool debug = false, QObject *parent = nullptr);
    ~ServerThread();
    void run() override;
    void SendData2Client( PostData postData);
private:
    Server* m_pServer;
    quint16 m_nPort;
    bool m_bDebug ;
    QObject *m_pParent ;

};


#endif
