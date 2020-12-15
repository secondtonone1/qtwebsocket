#include "protocol.h"
#include <QDebug>
//#include <QJsonObject>
#include <QJsonDocument>

QByteArray DeviceInfo::Pack(){
    QJsonObject tmp;
    tmp["sn"] = sn;
    tmp["type"] = type;
    tmp["mac"] = mac;
    tmp["ip"] = ip;
    tmp["version"] = version;
    QJsonDocument loadDoc(tmp);
    return loadDoc.toJson();
}

void DeviceInfo::Analysis(QString raw){
    QByteArray ba(raw.toStdString().c_str(),raw.toStdString().size());
    QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
    QJsonObject obj =  loadDoc.object();
    sn = obj["sn"].toString();
    type = obj["type"].toString();
    mac = obj["mac"].toString();
    ip = obj["ip"].toString();
    version = obj["version"].toString();
}

QByteArray StateInfo::Pack(){
    QJsonObject tmp;
    tmp["state"] = state;
    tmp["other"] = other;
    QJsonDocument loadDoc(tmp);
    return loadDoc.toJson();
}

void StateInfo::Analysis(QString raw){
    QByteArray ba(raw.toStdString().c_str(),raw.toStdString().size());
    QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
    QJsonObject obj =  loadDoc.object();
    state = obj["state"].toString();
    other = obj["other"].toString();
}




