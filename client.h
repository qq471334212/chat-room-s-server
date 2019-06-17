#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "loginwidget.h"
#include "chatwidget.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    virtual ~Client();

    void show();

signals:

private slots:
    void onRegisterSuccess(QString message);
    void onRegisterFailed(QString message);
    void onLoginSuccess(QString username, QString message);
    void onLoginFailed(QString message);
    void onQuit();

private:
    LoginWidget* loginWidget;
    ChatWidget* chatWidget;
};

#endif // CLIENT_H
