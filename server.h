#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include "mutexmap.h"
#include "postdata.h"
class RecvData;
class PostData;
QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
//using namespace std;
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, bool debug = false, QObject *parent = nullptr);
    ~Server();

    void SendData2Client(const PostData& postData);
    void PostData2Client(PostData& postData);
Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void onCloseServer();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void pong(quint64 elapsedTime, const QByteArray &payload);

private:
    void Analysis(std::string key, QString raw);
    QWebSocketServer *m_pWebSocketServer;
  //  QList<QWebSocket *> m_clients;
    MutexMap map_clients;
    bool m_debug;
public:
    QString filePath;
};

#endif // SERVER_H
