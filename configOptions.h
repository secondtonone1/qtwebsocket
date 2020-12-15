#ifndef CONFIGOPTIONS_H
#define CONFIGOPTIONS_H

#include <string>
#include <QString>

using namespace std;

enum eAccessType {
    UnkownAccessType = -1,      //未知
    OWLook                      //owlook平台
};

enum eTemperature {
    UnkownTemperature = -1,   //未知
    TempMeasurement           //测温模块
};

enum ePassType {
    UnkownPassType = -1,      //未知
    CardOrFace,               //刷卡或刷脸
    Card,                     //刷卡
    Face                      //刷脸
};


enum eLanguage {
    Chinese,                   //中文
    English                    //英文
};

class configOptions
{
public:
    configOptions();
    ~configOptions();

    void setFaceRecognition(bool status){bFaceRecgnition = status;}
    void setQRcode(bool status) {bQRcode = status;}
    void setPrivacyProtection(bool status) {bPrivacy = status;}
    void setMaskReminder(bool status) {bMask = status;}
    void setMotionDetection(bool status) {bMotionDetection = status;}
    void setTemperatureDisplay(bool status) {bTemperature = status;};
    void setWakeup(bool status) {bWakeup = status;}

    int setAccessType(int type);
    int setSvrAddr(QString addr);
    int setTemperature(int type);
    int setPassType(int type);
    int setLanguage(int type);

    void printAllOptions();

    bool getFaceRecognitionStatus() { return bFaceRecgnition;}
    bool getQRcodeStatus() { return bQRcode;}
    bool getPrivacyProtectionStatus() { return bPrivacy;}
    bool getMaskReminderStatus() {return bMask;}
    bool getMotionDetectionStatus() { return bMotionDetection;}
    bool getTemperatureDisplayStatus() { return bTemperature;};
    bool getWakeupStatus() {return bWakeup;}

    int getAccessType() {return int(accessType);}
    QString getSvrAddr() {return sSvrAddr;}
    int getTemperature() {return int(temperature);}
    int getPassType() {return int(passType);}
    int getLanguage() {return int(language);}

private:
    bool bFaceRecgnition;   //人脸识别
    bool bQRcode;           //扫码开门
    bool bPrivacy;          //隐私保护
    bool bMask;             //口罩提醒
    bool bMotionDetection;  //移动侦测
    bool bTemperature;      //温度提示
    bool bWakeup;           //智能感应唤醒

    eAccessType     accessType = OWLook; //接入方式
    QString          sSvrAddr;            //服务器地址

    eTemperature    temperature = TempMeasurement;  //测温模块
    ePassType       passType = CardOrFace;  //开门方式
    eLanguage       language = Chinese;     //语言选择
};


#endif // CONFIGOPTIONS_H
