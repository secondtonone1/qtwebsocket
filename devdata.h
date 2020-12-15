#ifndef DEVDATA_H
#define DEVDATA_H
#include<string>
#include <QtCore/qobject.h>
#include "constdef.h"
#include <QMetaType>

using namespace std;
class DevData:public QObject{
    Q_OBJECT
public:
    DevData(string ip, string mac, string sn,
             string devModel, string devType, string devKey):m_strMac(mac),
    m_strIp(ip),m_strSn(sn),m_strDevMd(devModel),m_strDevType(devType),m_strKey(devKey){
        m_nCfgState = 0;
    }

    DevData(){}
    DevData(const DevData& postData):QObject(nullptr){
        this->m_strMac = postData.m_strMac;
        this->m_strDevMd =postData.m_strDevMd;
        this->m_strIp = postData.m_strIp;
        this->m_strSn = postData.m_strSn;
        this->m_strDevType = postData.m_strDevType;
        this->m_nCfgState = postData.m_nCfgState;
        this->m_strKey = postData.m_strKey;
    }

    DevData & operator = (const DevData& postData){
        this->m_strMac = postData.m_strMac;
        this->m_strDevMd =postData.m_strDevMd;
        this->m_strIp = postData.m_strIp;
        this->m_strSn = postData.m_strSn;
        this->m_strDevType = postData.m_strDevType;
        this->m_nCfgState = postData.m_nCfgState;
        this->m_strKey = postData.m_strKey;
        return * this;
    }

     std::string GetBoxStr(){
        auto boxStr =m_strSn+BLANKSTR+ m_strMac + BLANKSTR + m_strIp+ BLANKSTR
                + m_strDevMd + BLANKSTR + BLANKSTR + BLANKSTR+ m_strDevType
                + BLANKSTR + BLANKSTR + BLANKSTR + CONVERT_CFG_STATE();
        return boxStr;
    }

    const std::string &GetSN(){
        return m_strSn;
    }

    const std::string &GetIP(){
        return m_strIp;
    }

    const std::string &GetKey(){
        return m_strKey;
    }

    string CONVERT_CFG_STATE(){
        cout << "m_nCfgState is " << m_nCfgState << endl;
        if (m_nCfgState == 0){
            return "连接成功";
        }

        if (m_nCfgState == 1){
            return "配置成功";
        }

        if (m_nCfgState == 2){
            return "配置失败";
        }

        return "未知错误";
    }

    void SetSN(const string & sn){
        m_strSn = sn;
    }
    void SetMac(const string & mac){
        m_strMac = mac;
    }
    void SetIp(const string & ip){
        m_strIp = ip;
    }
    void SetDevMd(const string & devMd){
        m_strDevMd = devMd;
    }
    void SetDevType(const string & devType){
        m_strDevType = devType;
    }
    void SetKey(const string & key){
        m_strKey = key;
    }

    void SetState(int state){
        m_nCfgState = state;
    }
private:

    //mac 地址
    string m_strMac;
    // ip 地址
    string m_strIp;
    // sn 码
    string m_strSn;
    // 设备型号
    string m_strDevMd;
    // 设备类型
    string m_strDevType;
    // 配置结果
    int m_nCfgState;
    // key
    string m_strKey;
};
Q_DECLARE_METATYPE(DevData);

#endif
