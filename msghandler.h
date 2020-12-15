#include "constdef.h"
#include <QObject>
#include <map>
#include <functional>
#include "singleton.h"
#include "recvdata.h"
#include "devdata.h"

typedef function<bool (const RecvData &)> handler;
class MsgHandler:public QObject, public CSingleton<MsgHandler>{

    friend class  CSingleton<MsgHandler>;
Q_OBJECT
public:
    ~MsgHandler();

    void RegMsgHandler(string _event, handler _handler);

    void InitHandlers();

    MsgHandler* GetRawPtr();

    void HandleMsg(string _event, const RecvData& recvData);
    MsgHandler(const MsgHandler &) = delete ;
    MsgHandler& operator=(const MsgHandler&)=delete;

    void EmitDevDataSig(const DevData& devData);
    void EmitStrSig(const string& strdata);
signals:
    void  devDataSig(DevData devData);
    void  strSig(QString data);
private:
     MsgHandler();

private:
   std::map<string, handler> m_mapHandlers;
};





