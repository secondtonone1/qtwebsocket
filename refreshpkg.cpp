#include "refreshpkg.h"
#include "ui_refreshpkg.h"
#include <iostream>
#include "iplistitem.h"
#include <map>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QLineEdit>
#include <string>
#include <QColor>
#include <QTime>
#include <vector>
#include <QSettings>
#include "postdata.h"
#include "uiposthread.h"
#include "singleton.h"
#include "postqueue.h"
#include "serverthread.h"
#include "recvthread.h"
#include "msghandler.h"
#include "posthread.h"
#include "QTimer"
using namespace std;
RefreshPkg::RefreshPkg(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RefreshPkg)
    ,m_nCurIndex(0)
{
    ui->setupUi(this);

    ui->pathApk->setPlaceholderText("请选择APK文件");
    ui->pathRom->setPlaceholderText("请选择APK文件");
    ui->pathRom->setReadOnly(true);
    ui->pathApk->setReadOnly(true);
    ui->textEditLog->setFontPointSize(10);

    //启动recv线程
    m_pRecvThread = new RecvThread(nullptr);
    m_pRecvThread->start();


    m_pWebSocketSrv = new ServerThread(WEBPORT, true, nullptr);

    //启动post线程
    m_pPostThread = new PostThread(nullptr, m_pWebSocketSrv);
    m_pPostThread->start();

    //启动webserver线程

    m_pWebSocketSrv->start();


    connect(this,&RefreshPkg::destroyed,this,&RefreshPkg::deadClose);
    connect(MsgHandler::getInstance()->GetRawPtr(), SIGNAL(devDataSig(DevData)), this, SLOT(devDataSlots(DevData)));
   // connect(MsgHandler::getInstance()->GetRawPtr(), SIGNAL(strSig(QString)), this, SLOT(strSigSlots(QString)));
}

void RefreshPkg::devDataSlots(DevData devData){
    cout << "receive dev data sig devData sn is " << devData.GetSN()<<endl;
    DevData itemData(std::move(devData));
    AddIpListItem(m_nCurIndex,itemData);
    m_nCurIndex++;
}

void RefreshPkg::strSigSlots(QString data){
    cout << "receive dev str is " << data.toStdString() << endl;
}


void RefreshPkg::deadClose(){

    cout << "deadClose" << endl;
    //卡死异常退出

    for(auto it = m_mapIPItems.begin(); it != m_mapIPItems.end(); it++){
        delete (it->second);
    }
    //以下代码导致析构会异常退出，所以注释了。
    //m_mapIPItems.clear();

    if (m_pRecvThread != nullptr){
        m_pRecvThread->StopLoop();
        while(!m_pRecvThread->CheckDead()){
            QThread::msleep(10);
        }
        delete(m_pRecvThread);
        m_pRecvThread = nullptr;
    }

    if (m_pPostThread != nullptr){
        m_pPostThread->StopLoop();
        while(!m_pPostThread->CheckDead()){
            QThread::msleep(10);
        }
        delete(m_pPostThread);
        m_pPostThread = nullptr;
    }

    if (m_pWebSocketSrv != nullptr){
        m_pWebSocketSrv->quit();
         m_pWebSocketSrv->wait();
        delete(m_pWebSocketSrv);
        m_pWebSocketSrv = nullptr;
    }

}

RefreshPkg::~RefreshPkg()
{
    delete ui;
}

//搜索回调逻辑
void RefreshPkg::on_searchBtn_clicked()
{

}

void RefreshPkg::AddIpListItem(int index, const DevData& postData){

   auto pItem = new IpListItem(this, index, postData);
   auto strKey = pItem->GetKey();

   m_mapIPItems.insert(pair<string,IpListItem*>(strKey, pItem));
   ui->ip_list_widget->addItem(pItem->GetListWidgetItem());
   ui->ip_list_widget->setItemWidget(pItem->GetListWidgetItem(), pItem->GetCheckBox());

}

void RefreshPkg::on_SelectorRom_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("选择ROM文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    //fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    fileDialog->setNameFilter(tr("ZIP or IMG Files (*.zip *.img)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }

    QString str = "已选择APK文件：";
    QString file;
    for(auto tmp:fileNames)
    {
        qDebug()<<tmp<<endl;
        ui->pathRom->setText(tmp);
        ui->pathRom->setReadOnly(false);
        ui->pathRom->setClearButtonEnabled(true);

        //string log = "已选择" + tmp.toStdString();
        //std::string str = "已选择ROM文件：";
        //tmp.prepend(QString::fromStdString(str));
        //ui->textEditLog->setText(tmp);
        //append(0, tmp);
        file += tmp + ", ";
    }
    append(0, str + file);


}

void RefreshPkg::on_SelectorApk_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("选择APK文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("APK Files(*.APK *.apk)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }

    QString str = "已选择APK文件：";
    QString file;
    for(auto tmp:fileNames)
    {
        qDebug()<<tmp<<endl;
        ui->pathApk->setText(tmp);
        ui->pathApk->setReadOnly(false);
        ui->pathApk->setClearButtonEnabled(true);

        //std::string str = "已选择APK文件：";
        //tmp.prepend(QString::fromStdString(str));
        //ui->textEditLog->setText(tmp);
        //append(0, tmp);
        file += tmp + ", ";
    }
    append(0, str + file);
    /*
    QMessageBox box;
    for (auto tmp:fileNames)
    {
        box.setText(tmp);
        box.setTextInteractionFlags(Qt::TextSelectableByMouse);
        box.exec();
     }
    */

}

//全选回调逻辑
void RefreshPkg::on_selectAll_clicked()
{
    for(auto it = m_mapIPItems.begin(); it != m_mapIPItems.end(); it++){
        if(it->second->GetCheckBox()->isChecked() == true){
            continue;
        }
        it->second->GetCheckBox()->setChecked(true);
        m_setSelectSN.insert(it->second->GetKey());
    }

}

void RefreshPkg::AddSelectedSN(string key){
    m_setSelectSN.insert(key);
}

void RefreshPkg::DelSelectedSN(string key){
    m_setSelectSN.erase(key);
}

void RefreshPkg::on_unselectAll_clicked()
{
    for(auto it = m_mapIPItems.begin(); it != m_mapIPItems.end(); it++){
        if(it->second->GetCheckBox()->isChecked() == false){
            continue;
        }
        it->second->GetCheckBox()->setChecked(false);
        m_setSelectSN.erase(it->second->GetKey());
    }

}

void RefreshPkg::on_pathRom_textEdited(const QString &arg1)
{
    qDebug() << arg1 << "已更改，请重新选择" << endl;
    if (arg1 == "")
    {
        ui->pathRom->setPlaceholderText("请重新选择");
        ui->pathRom->setReadOnly(true);
        std::string str = "已移除ROM文件";
        //ui->textEditLog->setText(QString::fromStdString(str));
        append(0, QString::fromStdString(str));
    } else {
        QMessageBox box;
        box.setText("已修改，请重新选择！");
        box.setTextInteractionFlags(Qt::TextSelectableByMouse);
        box.exec();
    }
}

void RefreshPkg::on_pathApk_textEdited(const QString &arg1)
{
    qDebug() << arg1 << "已更改，请重新选择" << endl;
    if(arg1 == "")
    {
        ui->pathApk->setPlaceholderText("请重新选择");
        ui->pathApk->setReadOnly(true);
        std::string str = "已移除ROM文件";
        //ui->textEditLog->setText(QString::fromStdString(str));
        append(0, QString::fromStdString(str));
    } else {
        QMessageBox box;
        box.setText("已修改，请重新选择！");
        box.setTextInteractionFlags(Qt::TextSelectableByMouse);
        box.exec();
    }
}

void RefreshPkg::append(int type, const QString &msg)
{
    qDebug() << "currentMsgCount:" << currentMsgCount << endl;
    if (currentMsgCount >= maxMsgCount) {
        ui->textEditLog->clear();
        currentMsgCount = 0;
    }
    //QString str;
    //str = "opertaion";

    ui->textEditLog->setTextColor(QColor(100,184,255));
    QDateTime current_date_time =QDateTime::currentDateTime();
    //QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
    //ui->textEditLog->append(QString("[%1] %2 %3").arg(current_date).arg(str).arg(msg));
    ui->textEditLog->append(QString("[%1] %2").arg(current_date).arg(msg));
    currentMsgCount++;

    QString logDirectory = QDir::currentPath() + "/LOG/";
    QDir dir(logDirectory);
    if (!dir.exists())
    {
        dir.mkdir(logDirectory);
    }
    QString logfile = logDirectory + userlog;
    qDebug() << "user log:" << logfile << endl;
    QFile file(logfile);
    if (!file.open(QFile::WriteOnly | QFile::Append))     //检测文件是否打开
    {
        QMessageBox::information(this, "Error Message", "Please Select a Text File!");
        return;
    }
    //file.setFileName(logfile);
    QTextStream stream(&logfile);
    qDebug() << "user log2:" << QString("[%1] %2").arg(current_date).arg(msg) << endl;
    //stream << QString("[%1] %2").arg(current_date).arg(msg) << "\r\n";
    //file.flush();
    //file.close();
    stream << ui->textEditLog->toPlainText();
}

//开始刷机
void RefreshPkg::on_pushButton_clicked()
{

    std::vector<PostData> vecPostData;
    //cout << "select vector size is " << m_setSelectSN.size() << endl;
    for(auto it = m_setSelectSN.begin(); it != m_setSelectSN.end();
        it ++){
        auto findIt = m_mapIPItems.find(*it);
        if (findIt == m_mapIPItems.end()){
            continue;
        }

        QJsonObject syncObj;
        QJsonObject dataObj;
        syncObj["event"] = "sc_syncfg";
        dataObj["faceRec"] = 1;
        dataObj["faceMask"] = 1;
        syncObj["data"] =dataObj;
       // vecPostData.push_back(PostData(findIt->second->GetKey(),"sc_syncfg", std::move(syncObj)));
        PostQueue<PostData>::getInstance()->push(PostData(findIt->second->GetKey(),"sc_syncfg", std::move(syncObj))  );
    }

   cout << "vecPostData size is " << vecPostData.size() << endl;
   //此处有连续点击bug
    //不要在按钮响应中启动线程，会造成连续点击卡死
    //auto postThread = new UIPostThread(std::move(vecPostData));
    //postThread->start();
    //postThread->deleteLater();

}

//人脸识别框开关
void RefreshPkg::on_checkBoxFace_clicked()
{
    qDebug() << "on_checkBoxFace_clicked:" << ui->checkBoxFace->checkState() << endl;
    bool status = ui->checkBoxFace->isChecked();
    qDebug() << "on_checkBoxFace_clicked:" << status << endl;

    options.setFaceRecognition(status);
    options.printAllOptions();
}

//扫码开门开关
void RefreshPkg::on_checkBoxScan_clicked()
{
  qDebug() << "on_checkBoxScan_clicked:" << ui->checkBoxScan->checkState() << endl;

  bool status = ui->checkBoxScan->isChecked();
  qDebug() << "on_checkBoxScan_clicked:" << status << endl;

  options.setQRcode(status);
  options.printAllOptions();
}

//隐私保护开关
void RefreshPkg::on_checkBoxPrivacy_clicked()
{
    qDebug() << "on_checkBoxPrivacy_clicked:" << ui->checkBoxPrivacy->checkState() << endl;

    bool status = ui->checkBoxPrivacy->isChecked();
    qDebug() << "on_checkBoxPrivacy_clicked:" << status << endl;

    options.setPrivacyProtection(status);
    options.printAllOptions();
}

//口罩提醒开关
void RefreshPkg::on_checkBoxMask_clicked()
{
    qDebug() << "on_checkBoxMask_clicked:" << ui->checkBoxMask->checkState() << endl;

    bool status = ui->checkBoxMask->isChecked();
    qDebug() << "on_checkBoxMask_clicked:" << status << endl;

    options.setMaskReminder(status);
    options.printAllOptions();
}


//移动侦测开关
void RefreshPkg::on_checkBoxMove_clicked()
{
    qDebug() << "on_checkBoxMove_clicked:" << ui->checkBoxMove->checkState() << endl;

    bool status = ui->checkBoxMove->isChecked();
    qDebug() << "on_checkBoxMove_clicked:" << status << endl;

    options.setMotionDetection(status);
    options.printAllOptions();
}

//温度显示开关
void RefreshPkg::on_checkBoxTemperature_clicked()
{
    qDebug() << "on_checkBoxTemperature_clicked:" << ui->checkBoxTemperature->checkState() << endl;

    bool status = ui->checkBoxTemperature->isChecked();
    qDebug() << "on_checkBoxTemperature_clicked:" << status << endl;

    options.setTemperatureDisplay(status);
    options.printAllOptions();
}

//智能感应唤醒
void RefreshPkg::on_checkBoxWakeup_clicked()
{
    qDebug() << "on_checkBoxWakeup_clicked:" << ui->checkBoxWakeup->checkState() << endl;

    bool status = ui->checkBoxWakeup->isChecked();
    qDebug() << "on_checkBoxWakeup_clicked:" << status << endl;

    options.setWakeup(status);
    options.printAllOptions();
}

//接入方式
void RefreshPkg::on_comboBoxAccess_activated(int index)
{
    qDebug() << "on_comboBoxAccess_activated:" << index << endl;

    options.setAccessType(index);

    options.printAllOptions();
}

//测温模块选择
void RefreshPkg::on_comboBoxTemperature_activated(int index)
{
    qDebug() << "on_comboBoxTemperature_activated:" << index << endl;

    options.setTemperature(index);
    options.printAllOptions();
}

//开门方式
void RefreshPkg::on_comboBoxDoor_activated(int index)
{
    qDebug() << "on_comboBoxDoor_activated:" << index << endl;

    options.setPassType(index);
    options.printAllOptions();
}

//语言选择
void RefreshPkg::on_comboBoxLanguage_activated(int index)
{
    qDebug() << "on_comboBoxLanguage_activated:" << index << endl;

    options.setLanguage(index);
    options.printAllOptions();
}


//将配置信息导出到配置文件
void RefreshPkg::on_pushButtonExport_clicked()
{
    //保存文件对话框，以供用户选择保存的文件名和路径
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
            tr("Save config"), "", tr("Config Files (*.ini)"));

   if (!fileName.isNull()) {
        qDebug() << "ini filename to save:" << fileName << endl;
        //保存文件


        QSettings settings(fileName, QSettings::IniFormat);
        settings.setIniCodec("UTF8");
        settings.setValue("/options/FaceRecognition", options.getFaceRecognitionStatus());
        settings.setValue("/options/QRCode", options.getQRcodeStatus());
        settings.setValue("/options/PrivacyProtection", options.getPrivacyProtectionStatus());
        settings.setValue("/options/MaskReminder", options.getMaskReminderStatus());
        settings.setValue("/options/MotionDetection", options.getMotionDetectionStatus());
        settings.setValue("/options/TemperatureDisplay", options.getTemperatureDisplayStatus());
        settings.setValue("/options/Wakeup", options.getWakeupStatus());
        settings.setValue("/options/AccessType", options.getAccessType());
        settings.setValue("/options/ServerAddr", options.getSvrAddr());
        settings.setValue("/options/TemperatureModule", options.getTemperature());
        settings.setValue("/options/PassType", options.getPassType());
        settings.setValue("/options/Language", options.getLanguage());

   } else {
       QMessageBox box;
       box.setText("没有导出配置文件：请选择保存的文件名及路径！");
       box.setTextInteractionFlags(Qt::TextSelectableByMouse);
       box.exec();
   }
}

//导入配置文件
void RefreshPkg::on_pushButtonImport_clicked()
{
    //选择配置文件（路径）
    QString file_name = QFileDialog::getOpenFileName(NULL,"选择配置文件",".", "*.ini");
    qDebug() << "FileName:" << file_name << endl;

    QSettings settings(file_name, QSettings::IniFormat);
    settings.setIniCodec("UTF8");

    //读取配置并根据配置信息设置UI组件状态
    Qt::CheckState state;

    //人脸识别
    bool bFaceRecognition = settings.value("/options/FaceRecognition").toBool();
    options.setFaceRecognition(bFaceRecognition);
    if (bFaceRecognition) {
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    //根据配置设置UI的相应状态
    ui->checkBoxFace->setCheckState(state);

    //扫码开门
    bool bQRCode = settings.value("/options/QRCode").toBool();
    options.setQRcode(bQRCode);
    if (bQRCode){
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    ui->checkBoxScan->setCheckState(state);

    //隐私保护
    bool bPrivacyProtection = settings.value("/options/PrivacyProtection").toBool();
    options.setPrivacyProtection(bPrivacyProtection);
    if (bPrivacyProtection){
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    ui->checkBoxPrivacy->setCheckState(state);

    //口罩提醒
    bool bMaskReminder = settings.value("/options/MaskReminder").toBool();
    options.setMaskReminder(bMaskReminder);
    if (bMaskReminder) {
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    ui->checkBoxMask->setCheckState(state);

    //移动侦测
    bool bMotionDetection = settings.value("/options/MotionDetection").toBool();
    options.setMotionDetection(bMotionDetection);
    if (bMotionDetection){
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    ui->checkBoxMove->setCheckState(state);

    //温度显示
    bool bTemperatureDisplay = settings.value("/options/TemperatureDisplay").toBool();
    options.setTemperatureDisplay(bTemperatureDisplay);
    if (bTemperatureDisplay) {
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    ui->checkBoxTemperature->setCheckState(state);

    //智能感应唤醒
    bool bWakeup = settings.value("/options/Wakeup").toBool();
    options.setWakeup(bWakeup);
    if (bWakeup) {
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    ui->checkBoxWakeup->setCheckState(state);

    //接入方式
    int iAccessType = settings.value("/options/AccessType").toInt();
    options.setAccessType(iAccessType);
    //设置显示值的索引，从0开始
    ui->comboBoxAccess->setCurrentIndex(iAccessType);

    //服务器地址
    QString svrAddr = settings.value("/options/ServerAddr").toString();
    options.setSvrAddr(svrAddr);
    ui->lineEditSvrAddr->setText(svrAddr);

    //测温模块
    int iTemperatureModule = settings.value("/options/TemperatureModule").toInt();
    options.setTemperature(iTemperatureModule);
    ui->comboBoxTemperature->setCurrentIndex(iTemperatureModule);

    //开门方式
    int iPassType = settings.value("/options/PassType").toInt();
    options.setPassType(iPassType);
    ui->comboBoxDoor->setCurrentIndex(iPassType);

    //语言选择
    int iLanguage = settings.value("/options/Language").toInt();
    options.setLanguage(iLanguage);
    ui->comboBoxLanguage->setCurrentIndex(iLanguage);

    //debug信息
    options.printAllOptions();
}
