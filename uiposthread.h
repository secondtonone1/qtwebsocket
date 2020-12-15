#ifndef UI_POST_THREAD_H
#define UI_POST_THREAD_H
#include <QThread>
#include <QtWidgets/QMainWindow>
#include "postdata.h"
#include <string>
#include <iostream>
#include <postqueue.h>
using namespace std;
//ui 投递消息给发送队列
class UIPostThread:public QThread{
    Q_OBJECT
public:
    UIPostThread(vector<PostData> vecPost){
        m_vecPostData = vecPost;
    }
    void run() override{

        for(auto it = m_vecPostData.begin(); it != m_vecPostData.end();
            it ++){
            cout << "key is: " << it->GetKey() << " cmd is: " << it->GetCmd() << endl;
            auto p1 = PostQueue<PostData>::getInstance();
            p1->push(PostData(*it));
        }

    }
private:
    std::vector<PostData> m_vecPostData;
};


#endif
