#include "posthread.h"
#include "msghandler.h"
#include "postqueue.h"
#include "serverthread.h"
   PostThread::PostThread(QObject *parent, ServerThread * sth):QThread(parent){
        m_bStop = false;
        m_bDead = false;
        m_pServerThread = sth;
    }

   PostThread:: ~PostThread(){
        cout << "post Thread exit success !" << endl;
    }
    void PostThread::StopLoop(){
        std::lock_guard<std::mutex> locker(mu_);
        m_bStop = true;
        cout << "set post thread stop flag " << endl;
    }

    bool PostThread::CheckDead(){
        std::lock_guard<std::mutex> locker(mu_);
        return m_bDead;
    }

    void PostThread::KillThread(){
        std::lock_guard<std::mutex> locker(mu_);
        cout << "kill post thread" <<endl;
        m_bDead = true;
    }

    bool PostThread::CheckStop(){
         std::lock_guard<std::mutex> locker(mu_);
         if (m_bStop){
             return true;
         }

         return false;
    }

    void PostThread::run() {
           while(!CheckStop()){
                auto postQue = PostQueue<PostData>::getInstance();
                PostData postData;
                auto res = postQue->wait_and_pop(postData, 10);
                if (res == -1){
                    QThread::msleep(10);
                   // cout << "queue is empty, wait 10 ms" << endl;
                    continue;
                }
                cout << "get from postque data is "<< postData.GetKey() << endl;
                m_pServerThread->SendData2Client(std::move(postData));
           }

           cout << "post thread exit" << endl;
           KillThread();
    }


