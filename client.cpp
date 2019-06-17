#include "client.h"
#include "clientsocketmanager.h"
#include <QMessageBox>

Client::Client(QObject *parent)
    : QObject(parent),
      loginWidget(new LoginWidget),
      chatWidget(new ChatWidget)
{
    ClientSocketManager::getInstance()->initSocket();
    const ClientSocket* socket = ClientSocketManager::getInstance()->getClientSocket();

    QObject::connect(socket, SIGNAL(sendMessage(QString)),                                      //客户端监听发过来的数据包
                     chatWidget, SLOT(onSendMessage(QString)));

    QObject::connect(socket, SIGNAL(registerSuccess(QString)),                                  //注册成功
                     this, SLOT(onRegisterSuccess(QString)));
    QObject::connect(socket, SIGNAL(registerFailed(QString)),                                   //注册失败
                     this, SLOT(onRegisterFailed(QString)));

    QObject::connect(socket, SIGNAL(loginSuccess(QString,QString)),                             //登陆成功
                     this, SLOT(onLoginSuccess(QString,QString)));
    QObject::connect(socket, SIGNAL(loginFailed(QString)),                                      //登陆失败
                     this, SLOT(onLoginFailed(QString)));
    QObject::connect(socket, SIGNAL(quit()), this, SLOT(onQuit()));                             //退出信号

    QObject::connect(socket, SIGNAL(chat(QString)), chatWidget, SLOT(onChat(QString)));         //聊天包

    QObject::connect(socket, SIGNAL(refreshUser(QStringList)), chatWidget, SLOT(onRefreshUser(QStringList)));   //刷新房间用户
}

Client::~Client()
{
    delete loginWidget;
    delete chatWidget;
}

void Client::show()
{
    loginWidget->show();
}

void Client::onRegisterSuccess(QString message)
{
    QMessageBox::information(loginWidget, "注册信息", message);
}

void Client::onRegisterFailed(QString message)
{
    QMessageBox::warning(loginWidget, "注册信息", message);
}

void Client::onLoginSuccess(QString username, QString message)
{
    QMessageBox::information(loginWidget, "登录信息", message);
    loginWidget->hide();
    chatWidget->setUsername(username);
    chatWidget->show();
}

void Client::onLoginFailed(QString message)
{
    QMessageBox::warning(loginWidget, "登录信息", message);
}

void Client::onQuit()
{
    chatWidget->hide();
    loginWidget->show();
}
