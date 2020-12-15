#include"iplistitem.h"
#include "constdef.h"
#include <iostream>
#include "refreshpkg.h"

using namespace std;
IpListItem::IpListItem(RefreshPkg* refresh,int index, const DevData & postData):m_nIndex(index)
{
    m_pRefreshPkg = refresh;
    m_pListWidgetItem = new QListWidgetItem();
    m_postData = postData;
    m_pCheckBox = new QCheckBox();

    auto boxStr = m_postData.GetBoxStr();
    m_pCheckBox->setText(boxStr.c_str());
    m_pCheckBox->setCheckable(true);
    //把所有checkBox的信号都引向同一个槽
    connect(m_pCheckBox, SIGNAL(stateChanged(int)), this, SLOT(anyStateChanged(int)));
}

IpListItem::~IpListItem(){
    if (m_pCheckBox != nullptr){
            delete m_pCheckBox;
              m_pCheckBox = nullptr;
    }

    if (m_pListWidgetItem != nullptr){
        delete  m_pListWidgetItem;
        m_pListWidgetItem = nullptr;
    }

    m_pRefreshPkg = nullptr;
}

/*
string IpListItem::GetSN(){
return m_postData.GetSN();
}*/

string IpListItem:: GetKey(){
    return m_postData.GetKey();
}
string IpListItem::GetIP(){
    return m_postData.GetIP();
}

QCheckBox * IpListItem::GetCheckBox(){
    return m_pCheckBox;
}

QListWidgetItem * IpListItem::GetListWidgetItem(){
    return m_pListWidgetItem;
}

void IpListItem::anyStateChanged(int state){
    if(state == Qt::Checked){
        cout << "has selected " << m_postData.GetKey() << endl;
        if (!m_pRefreshPkg){
            return ;
        }

        m_pRefreshPkg->AddSelectedSN(m_postData.GetKey());
    }else{
        cout << "has canceled selected " << m_postData.GetKey() << endl;
        m_pRefreshPkg->DelSelectedSN(m_postData.GetKey());
    }
}
