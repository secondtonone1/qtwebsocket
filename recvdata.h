#ifndef RECVDATA_H
#define RECVDATA_H
#include<string>
#include <QtCore/qobject.h>
#include "constdef.h"
#include <QJsonObject>
#include "server.h"

using namespace std;
class RecvData:public QObject{
    friend Server;
    Q_OBJECT
public:
    RecvData(string key, string cmd,  QJsonObject obj):
    m_strKey(key),m_strCmd(cmd),m_objJson(obj){
    }
    RecvData(){}
    RecvData(const RecvData& recvData):QObject(nullptr){
        this->m_strKey = recvData.m_strKey;
        this->m_strCmd = recvData.m_strCmd;
        this->m_objJson = recvData.m_objJson;
    }

    RecvData & operator = (const RecvData& recvData){
        this->m_strKey = recvData.m_strKey;
        this->m_strCmd = recvData.m_strCmd;
        this->m_objJson = recvData.m_objJson;
        return * this;
    }

    bool operator < (const RecvData& recvData){
        return this->m_strKey < recvData.m_strKey;
    }

    std::string GetKey() const{
        return m_strKey;
    }

    std::string GetCmd(){
        return m_strCmd;
    }

    QJsonObject  GetJsonObject() const {
        return m_objJson;
    }
private:

    // ip 地址
    string m_strKey;
    // cmd 码
    string m_strCmd;
    // data
    string m_strData;

    //JsonObject
    QJsonObject m_objJson;
};

#endif
