#include "configOptions.h"
#include <QDebug>
#include <QSettings>

configOptions::configOptions()
{

}

configOptions::~configOptions()
{

}

int configOptions::setAccessType(int type)
{
    int res = 0;

    switch(eAccessType(type)){
    case OWLook:
        accessType = eAccessType(type);
        break;
    default:
        accessType = UnkownAccessType;
        qCritical() << "setAccessType() type error: " << type << endl;
        res = 1;
    }

    return res;
}


int configOptions::setSvrAddr(QString addr)
{
    int res = 0;

    //TODO：校验

    sSvrAddr = addr;
    return res;
}

int configOptions::setTemperature(int type)
{
    int res = 0;
    switch(eTemperature(type)){
    case TempMeasurement:
        temperature = TempMeasurement;
        break;
    default:
        temperature = UnkownTemperature;
        qCritical() << "setTemperature() type error: " << type << endl;
        res = 1;
    }
    return res;
}


int configOptions::setPassType(int type)
{
    int res = 0;
    switch(ePassType(type)){
    case CardOrFace:
    case Card:
    case Face:
        passType = ePassType(type);
        break;
    default:
        passType = UnkownPassType;
        qCritical() << "setPassType() type error: " << type << endl;
        res = 1;
    }
    return res;
}

int configOptions::setLanguage(int type)
{
    int res = 0;

    switch(eLanguage(type)){
    case Chinese:
    case English:
        language = eLanguage(type);
        break;
    default:
        qWarning() << "setPassType() type error: " << type << endl;
        res = 1;
        language = Chinese;
    }
    return res;
}

void configOptions::printAllOptions()
{
    QString OnStatus = "已选择(打开)", OffStatus = "未选择(关闭)";

    qDebug() << "PrintAllOptions: " << endl;

    qDebug() << "人脸识别：";
    if (bFaceRecgnition)
    {
        qDebug() << "\t"<< OnStatus << endl;
    } else {
        qDebug() << "\t" << OffStatus << endl;
    }

    qDebug() << "扫码开门：";
    if (bQRcode)
    {
        qDebug() << "\t" << OnStatus << endl;
    } else {
        qDebug() << "\t" << OffStatus << endl;
    }

    qDebug() << "隐私保护：";
    if (bPrivacy)
    {
        qDebug() << "\t" << OnStatus << endl;
    } else {
        qDebug() << "\t" << OffStatus << endl;
    }

    qDebug() << "口罩提醒：";
    if(bMask)
    {
        qDebug() << "\t" << OnStatus << endl;
    } else {
        qDebug() << "\t" << OffStatus << endl;
    }

    qDebug() << "移动侦测：";
    if(bMotionDetection)
    {
        qDebug() << "\t" << OnStatus << endl;
    } else {
        qDebug() << "\t" << OffStatus << endl;
    }

    qDebug() << "温度提示：";
    if (bTemperature)
    {
        qDebug() << "\t" << OnStatus << endl;
    } else {
        qDebug() << "\t" << OffStatus << endl;
    }

    qDebug() << "智能感应唤醒：";
    if(bWakeup)
    {
        qDebug() << "\t" << OnStatus << endl;
    } else {
        qDebug() << "\t" << OffStatus << endl;
    }

    qDebug() << "接入方式：";
    switch (accessType) {
    case OWLook:
        qDebug() << "\t" << OWLook << " (OWLook平台)" << endl;
        break;
    default:
        qDebug() << "\t" << "未知平台" << endl;
    }

    qDebug() << "服务器地址：" << sSvrAddr << endl;

    qDebug() << "测温模块：";
    switch(temperature){
    case TempMeasurement:
        qDebug() << "\t" << TempMeasurement << "(温度测量)" << endl;
        break;
    default:
        qDebug() << "\t" << "未知模块" << endl;
    }

    qDebug() << "开门方式：";
    switch (passType) {
    case CardOrFace:
        qDebug() << "\t" << passType << "(刷卡或刷脸)" << endl;
        break;
    case Card:
        qDebug() << "\t" << passType << "(刷卡)" << endl;
        break;
    case Face:
        qDebug() << "\t" << passType << "(刷脸)" << endl;
        break;
    default:
        qDebug() << "\t" << "未知" << endl;
    }

    qDebug() << "语言选择：";
    switch(language){
    case Chinese:
        qDebug() << "\t" << language << "(中文)" << endl;
        break;
    case English:
        qDebug() << "\t" << language << "(English)" << endl;
        break;
    default:
        qDebug() << "\t" << "未知" << endl;
    }
}


