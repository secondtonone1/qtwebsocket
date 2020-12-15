#include "serverthread.h"

ServerThread::ServerThread(quint16 port, bool debug , QObject *parent){
    m_nPort = port;
    m_bDebug = debug;
    m_pParent = parent;
    m_pServer = nullptr;

}

ServerThread:: ~ServerThread(){
    cout << "server thread exit success!"<<endl;
}

void ServerThread::SendData2Client(PostData postData){
    m_pServer->SendData2Client(postData);
}

void ServerThread::run(){
    m_pServer = new Server(m_nPort,m_bDebug, m_pParent);
    exec();
    cout << "destruct m_pServer data" << endl;
    if (m_pServer != nullptr){
        delete m_pServer;
        m_pServer = nullptr;
    }
}


