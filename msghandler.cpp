#include "msghandler.h"

#include "recvdata.h"
#include "recvthread.h"
MsgHandler::MsgHandler(){

}

MsgHandler::~MsgHandler(){

}

void MsgHandler::RegMsgHandler(string _event, handler _handler){
    m_mapHandlers.insert(pair<string, handler>(_event, _handler));
}

MsgHandler* MsgHandler::GetRawPtr(){
    return m_pInstance.get();
}


void MsgHandler::HandleMsg(string _event, const RecvData& recvData){
    auto iter =m_mapHandlers.find(_event);
    if(iter == m_mapHandlers.end()){
        return;
    }
    auto bRes = iter->second(recvData);
    if(!bRes){
        cout << "handle msg " << _event << " failed" << endl;
        return;
    }
    cout << "handle msg " << _event << " success" << endl;

    return ;
}

void MsgHandler::EmitDevDataSig(const DevData& devData){
     emit devDataSig(devData);
}

void MsgHandler::EmitStrSig(const string& strdata){
    emit strSig(QString(strdata.c_str()));
}

handler cs_syncinfo = [](const RecvData& recvData)->bool{
    cout << "this is cs_syncinfo function" << endl;

    DevData devData;
    devData.SetMac(recvData.GetJsonObject()["mac"].toString().toStdString());
    devData.SetSN(recvData.GetJsonObject()["sn"].toString().toStdString());
    devData.SetIp(recvData.GetJsonObject()["ip"].toString().toStdString());
    devData.SetDevMd(recvData.GetJsonObject()["devModel"].toString().toStdString());
    devData.SetDevType(recvData.GetJsonObject()["devType"].toString().toStdString());
    devData.SetKey(recvData.GetKey());
    devData.SetState(0);
    cout << "begin send dev data sig" <<endl;
    MsgHandler::getInstance()->EmitDevDataSig(devData);

    return true;
};

void MsgHandler:: InitHandlers(){
    RegMsgHandler("cs_syncinfo", cs_syncinfo);
}


