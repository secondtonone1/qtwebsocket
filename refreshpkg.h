#ifndef REFRESHPKG_H
#define REFRESHPKG_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include "devdata.h"
#include "postdata.h"
#include "configOptions.h"
#include <QMap>

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class RefreshPkg; }
QT_END_NAMESPACE
class IpListItem;
class ServerThread;
class RecvThread;
class PostThread;
class UIPostThread;
class RefreshPkg : public QMainWindow
{
    Q_OBJECT

public:
    RefreshPkg(QWidget *parent = nullptr);
    ~RefreshPkg();

    void AddIpListItem(int index, const DevData& postData);
    void AddSelectedSN(string sn);
    void append(int type, const QString &msg);
    void DelSelectedSN(string sn);

private slots:
    void on_searchBtn_clicked();

    void on_SelectorRom_clicked();

    void on_SelectorApk_clicked();

    void on_selectAll_clicked();

    void on_unselectAll_clicked();

    void on_pathRom_textEdited(const QString &arg1);

    void on_pathApk_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    void deadClose();

public slots:
    void devDataSlots(DevData devData);
    void strSigSlots(QString data);

    void on_checkBoxFace_clicked();

    void on_comboBoxAccess_activated(int index);

    void on_checkBoxScan_clicked();

    void on_checkBoxPrivacy_clicked();

    void on_checkBoxMask_clicked();

    void on_checkBoxMove_clicked();

    void on_checkBoxTemperature_clicked();

    void on_checkBoxWakeup_clicked();

    void on_comboBoxTemperature_activated(int index);

    void on_comboBoxDoor_activated(int index);

    void on_comboBoxLanguage_activated(int index);

    void on_pushButtonExport_clicked();

    void on_pushButtonImport_clicked();

private:
    Ui::RefreshPkg *ui;
    std::map<string, IpListItem*> m_mapIPItems;

    std::set<string> m_setSelectSN;

    //用户操作日志文件名
    QString userlog = "user.log";

    ServerThread * m_pWebSocketSrv;

    uint32_t currentMsgCount = 0;
    uint32_t maxMsgCount = 100;

    configOptions options;


    RecvThread * m_pRecvThread;
    PostThread * m_pPostThread;
    UIPostThread * m_pUIPostThread;
    qint32 m_nCurIndex;
};
#endif // REFRESHPKG_H
