#ifndef MUTEXMAP_H
#define MUTEXMAP_H
#include <QMap>
#include <QMutex>
#include "QtWebSockets/qwebsocket.h"
#include "postdata.h"
#include <QMutexLocker>
#include <QJsonDocument>
class MutexMap {
public:

    void AddItem(QString k,QWebSocket *v){
        m_mutex.lock();
        m_map[k] = v;
        m_mutex.unlock();
    }

void DeleteItem(QString k){

        QMutexLocker locker(&m_mutex);
        QMap<QString,QWebSocket *>::iterator i = m_map.find(k);
        if (i == m_map.end()){
            return ;
        }

        delete i.value();
        m_map.erase(i);
    }

    QWebSocket *FindItem(QString k){
         m_mutex.lock();
         QMap<QString,QWebSocket *>::iterator i = m_map.find(k);
         if (i == m_map.end()){
             return nullptr;
         }
         m_mutex.unlock();
         return i.value();
    }

    void PrintInfo(){
        m_mutex.lock();
        QMap<QString,QWebSocket *>::iterator beg = m_map.begin();
        while (beg != m_map.end()){
            qDebug() << "key:" <<beg.key()<<"value:"<<beg.value();
            beg++;
        }
        m_mutex.unlock();
    }

    void ClearAll(){
        m_mutex.lock();
        qDeleteAll(m_map.begin(),m_map.end());
        m_map.clear();
        m_mutex.unlock();
    }

    void DeleteAllLater(){
        m_mutex.lock();
        QMap<QString,QWebSocket *>::iterator beg = m_map.begin();
        while (beg != m_map.end()){
           beg.value()->deleteLater();
           beg = m_map.erase(beg);
        }
        m_mutex.unlock();
    }

    void SafeSendData(const PostData& postData){
        QMutexLocker locker(&m_mutex);
        QMap<QString,QWebSocket *>::iterator i = m_map.find(QString(postData.GetKey().c_str()));
        if (i == m_map.end()){
            return;
        }


        auto jsonObj = postData.GetJsonObject();
        QJsonDocument loadDoc(jsonObj);
        auto bJson = loadDoc.toJson();
        cout << "send data is " << QString(bJson).toStdString() << endl;
        auto nSend = i.value()->sendTextMessage(bJson);
        i.value()->flush();
        cout << "send length is " << nSend << endl;
    }
private:
    QMutex m_mutex;
    QMap<QString,QWebSocket *> m_map;

};

#endif // MUTEXMAP_H
