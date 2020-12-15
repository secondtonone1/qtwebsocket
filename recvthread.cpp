#include "recvthread.h"
#include "msghandler.h"

   RecvThread::RecvThread(QObject *parent ):QThread(parent){
        m_bStop = false;
        m_bDead = false;
    }

   RecvThread:: ~RecvThread(){
        cout << "Recv Thread exit success !" << endl;
    }
    void RecvThread::StopLoop(){
        std::lock_guard<std::mutex> locker(mu_);
        m_bStop = true;
        cout << "set recv thread stop flag " << endl;
    }

    bool RecvThread::CheckDead(){
        std::lock_guard<std::mutex> locker(mu_);
        return m_bDead;
    }

    void RecvThread::KillThread(){
        std::lock_guard<std::mutex> locker(mu_);
        cout << "kill recv thread" <<endl;
        m_bDead = true;
    }

    bool RecvThread::CheckStop(){
         std::lock_guard<std::mutex> locker(mu_);
         if (m_bStop){
             return true;
         }

         return false;
    }

    void RecvThread::run() {
           while(!CheckStop()){
                auto recvQue = RecvQueue<RecvData>::getInstance();
                RecvData recvData;
                auto res = recvQue->wait_and_pop(recvData, 10);
                if (res == -1){
                    QThread::msleep(10);
                   // cout << "queue is empty, wait 10 ms" << endl;
                    continue;
                }
                cout << "get from recvque data is "<< recvData.GetKey() << endl;
                MsgHandler::getInstance()->HandleMsg(recvData.GetCmd(), recvData);
           }

           cout << "recv thread exit" << endl;
           KillThread();
    }


