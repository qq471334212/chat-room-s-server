#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QStringList>
#include "protocol.h"

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QTcpSocket* socket = nullptr, QObject *parent = nullptr);

    inline quint64 write(QByteArray array){
        return socket->write(array);
    }

    inline bool isValid()const{return socket->isValid();}
signals:
    void sendMessage(QString);
    void registerSuccess(QString);
    void registerFailed(QString);
    void loginSuccess(QString, QString);
    void loginFailed(QString);
    void quit();
    void chat(QString);
    void refreshUser(QStringList);

private slots:
    void onConnectToHost();
    void onConnected();
    void onDisconnected();
    void onReadyRead();

private:
    QTcpSocket* socket;
    QTimer* timer;
    QByteArray buffer;

    void registerHandler(const Protocol& protocol);
    void loginHandler(const Protocol& protocol);
    void quitHandler(const Protocol& protocol);
    void chatHandler(const Protocol& protocol);
    void refreshUserHandler(const Protocol& protocol);
};

#endif // CLIENTSOCKET_H
