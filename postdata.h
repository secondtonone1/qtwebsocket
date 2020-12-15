#ifndef POSTDATA_H
#define POSTDATA_H
#include<string>
#include <QtCore/qobject.h>
#include <QJsonObject>
#include "constdef.h"

using namespace std;

class PostData:public QObject{

    Q_OBJECT
public:
    PostData(string key, string cmd,  QJsonObject obj):
    m_strKey(key),m_strCmd(cmd),m_objJson(obj){
    }
    PostData(){}
    PostData(const PostData& postData):QObject(nullptr){
        this->m_strKey = postData.m_strKey;
        this->m_strCmd = postData.m_strCmd;
        this->m_objJson = postData.m_objJson;
    }

    PostData & operator = (const PostData& postData){
        this->m_strKey = postData.m_strKey;
        this->m_strCmd = postData.m_strCmd;
        this->m_objJson = postData.m_objJson;
        return * this;
    }

    bool operator < (const PostData& postData){
        return this->m_strKey < postData.m_strKey;
    }

    std::string GetKey() const {
        return m_strKey;
    }

    std::string GetCmd() const {
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
