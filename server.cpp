#include "server.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <iostream>
#include <QString>
#include "constdef.h"
#include "recvdata.h"
#include "recvqueue.h"
#include "msghandler.h"
#include "postdata.h"
#include "postqueue.h"
using namespace std;
QT_USE_NAMESPACE

Server::Server(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
    QWebSocketServer::NonSecureMode, this)),
    m_debug(debug)
{
    MsgHandler::getInstance()->InitHandlers();

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "Echoserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,this, &Server::onNewConnection);
        //connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
       connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::onCloseServer);
    }else{
        cout << "server init failed" << endl;
    }

    cout << "websocket server start success" << endl;
}

void Server::onCloseServer(){
    cout << "web socket server closed success!" << endl;
}

Server::~Server()
{
    m_pWebSocketServer->close();
    if (m_pWebSocketServer != nullptr){
            delete (m_pWebSocketServer);
            m_pWebSocketServer = nullptr;
    }
    map_clients.ClearAll();

    cout << "websocket server exit success" << endl;
}

void Server::SendData2Client(const PostData& postData){
   map_clients.SafeSendData(postData);
}

void Server::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    qDebug() << pSocket->peerAddress().toString();
    qDebug() << pSocket->peerPort();
    cout << "new connection " << endl;
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);
    QString key = pSocket->peerAddress().toString()+ QString::number(pSocket->peerPort(),10);
    cout << "new connection is " << key.toStdString() << endl;
    map_clients.AddItem(key,pSocket);
  //  m_clients << pSocket;
    map_clients.PrintInfo();
}


void Server::pong(quint64 elapsedTime, const QByteArray &payload){
    qDebug() << "elapsedTime:" <<elapsedTime << "payload:" <<payload;
}

void Server::Analysis(std::string key, QString raw){
    QByteArray ba(raw.toStdString().c_str(),raw.toStdString().size());
    QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
    QJsonObject obj =  loadDoc.object();
    RecvData recvData;
    recvData.m_strCmd = obj["event"].toString().toStdString();
    recvData.m_strKey = key;
    recvData.m_objJson = obj["data"].toObject();
    RecvQueue<RecvData>::getInstance()->push(recvData);
}

void Server::PostData2Client(PostData &postData){
    PostQueue<PostData>::getInstance()->push(move(postData));
}

void Server::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    cout << "Message received:" << message.toStdString() << endl;
    if (m_debug)
        qDebug() << "Message received:" << message;
    if (pClient) {
        QString key = pClient->peerAddress().toString()+ QString::number(pClient->peerPort(),10);
        auto ret = map_clients.FindItem(key);
        //ret->sendTextMessage(message);
        Analysis(key.toStdString(), message);

        QJsonObject obj;
        obj["event"] = "sc_syncinfo";
        QJsonObject subObj;
        subObj["sn"] = "10247788";
        obj["data"]= subObj;
        auto postData = PostData(key.toStdString(),"sc_syncinfo", move(obj));
        PostData2Client(postData);
    }
}

void Server::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        qDebug() <<"peerAddress:"<< pClient->peerAddress() <<endl;
        QString key = pClient->peerAddress().toString()+ QString::number(pClient->peerPort(),10);
        map_clients.DeleteItem(key);
        map_clients.PrintInfo();
    }
}
