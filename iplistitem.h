#ifndef IPLISTITEM_H
#define IPLISTITEM_H
#include <QListWidgetItem>
#include <string>
#include <iostream>
#include <QCheckBox>
#include <QWidget>
#include "devdata.h"
class RefreshPkg;
using namespace std;
class IpListItem: public QWidget{
    Q_OBJECT
public:
    IpListItem(RefreshPkg* refresh, int index, const DevData & postData);
    ~IpListItem();
    //string GetSN();
    string GetIP();
    string GetKey();
    QCheckBox * GetCheckBox();
    QListWidgetItem* GetListWidgetItem();
private slots:
    void anyStateChanged(int);
private:
    int m_nIndex;
    // 复选框
    QCheckBox *m_pCheckBox;
    //listwidget的Item
    QListWidgetItem * m_pListWidgetItem;
    RefreshPkg * m_pRefreshPkg;
    DevData m_postData;
};

#endif // IPLISTITEM_H
