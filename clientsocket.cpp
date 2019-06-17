#include "clientsocket.h"
#include <QHostAddress>
#include <QDateTime>
#include <QJsonArray>

ClientSocket::ClientSocket(QTcpSocket* socket, QObject *parent)
    : QObject(parent),
      socket(socket),
      timer(new QTimer(this))
{
    if(this->socket == nullptr){
        //1. 创建套接字
        this->socket = new QTcpSocket(this);
    }

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onConnectToHost()));              //绑定掉线时定时重连
    QObject::connect(this->socket, SIGNAL(connected()), this, SLOT(onConnected()));         //绑定新的连接信号
    QObject::connect(this->socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));   //绑定断开信号
    QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));         //绑定读取信号的信号
    //2. 建立连接
    if(!this->socket->isValid()){
        timer->start(2000);
    }
}

void ClientSocket::onConnectToHost()        //绑定服务器IP和端口
{
    socket->connectToHost(QHostAddress("192.168.2.11"), 10005);
}

void ClientSocket::onConnected()            //断线重连成功时,断开计时器
{
    qDebug()<<"连接成功";
    timer->stop();
}

void ClientSocket::onDisconnected()         //掉线时,开启计时器
{
    timer->start(2000);
}

void ClientSocket::onReadyRead()
{
    buffer += socket->readAll();

    while(1){
        QPair<bool, Protocol> ret = Protocol::unpack(buffer);   //一直读取数据流
        if(ret.first == false)  return;                         //当解压的包是一个完整的包时,继续后面

        Protocol& p = ret.second;                               //把第二个读出来的数据包读出来

        switch(p.getType()){
        case Protocol::refreshUser:
            this->refreshUserHandler(p);    //刷新用户
            break;
        case Protocol::chat:
            this->chatHandler(p);
            break;
        case Protocol::quit:
            this->quitHandler(p);
            break;
        case Protocol::login:
            this->loginHandler(p);
            break;
        case Protocol::none:
            break;
        case Protocol::regist:
            this->registerHandler(p);
            break;
        }
    }
}

void ClientSocket::registerHandler(const Protocol& protocol)        //注册包
{
    QString type = protocol["type"].toString();
    if(type == "success"){
        emit registerSuccess(protocol["message"].toString());
    }else if(type == "failed"){
        emit registerFailed(protocol["message"].toString());
    }else{}
}


void ClientSocket::loginHandler(const Protocol& protocol)           //登陆包
{
    QString type = protocol["type"].toString();
    if(type == "success"){
        emit loginSuccess(protocol["username"].toString() ,protocol["message"].toString());
    }else if(type == "failed"){
        emit loginFailed(protocol["message"].toString());
    }else{}
}

void ClientSocket::quitHandler(const Protocol& protocol)            //退出包
{
    Q_UNUSED(protocol);

    emit quit();
}

void ClientSocket::chatHandler(const Protocol& protocol)            //聊天包
{
    QString message = protocol["username"].toString();  //获取用户名
    message += " ";
    message += QDateTime::currentDateTime().toString(); //获取的系统时间
    message += ":\n";
    message += protocol["chat"].toString();             //获取的聊天数据

    emit chat(message);
}

void ClientSocket::refreshUserHandler(const Protocol &protocol)     //刷新用户
{
    QJsonArray array = protocol["user"].toArray();
    QStringList list;
    int i = 0;
    for(i = 0; i < array.size(); i++){
        list.append(array[i].toString());
    }

    emit refreshUser(list);
}
