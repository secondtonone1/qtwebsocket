#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QJsonObject>

class Protocol
{
public:
    Protocol(){};
    virtual QByteArray Pack() = 0;
    virtual void Analysis(QString raw) = 0;
};

class DeviceInfo : public Protocol{
public:
    QString sn;
    QString type;
    QString mac;
    QString ip;
    QString version;
    DeviceInfo(){};
    QByteArray Pack();
    void Analysis(QString raw);
};

class StateInfo :public Protocol{
public:
    QString state;
    QString other;
    StateInfo();
    QByteArray Pack();
    void Analysis(QString raw);
};


#endif // PROTOCOL_H
