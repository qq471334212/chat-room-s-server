#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "clientsocketmanager.h"

ChatWidget::ChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::onChat(QString text)
{
    ui->tbChat->append(text);
}

void ChatWidget::on_leChat_returnPressed()
{
    //获取内容
    QString text = ui->leChat->text();
    ui->leChat->clear();

    //发送给服务器
    Protocol p;
    p.setType(Protocol::chat);
    p["roomname"] = "";
    p["username"] = username;
    p["chat"] = text;

    ClientSocketManager::getInstance()->write(p.toByteArray());
}




void ChatWidget::onRefreshUser(QStringList list)
{
    ui->lwUser->clear();
    ui->lwUser->addItems(list);
}




void ChatWidget::closeEvent(QCloseEvent *event)
{
    if(ClientSocketManager::getInstance()->isValid()){
        //网络正常
        event->ignore();
    }else{
        //网络不正常
        event->accept();
        return;
    }

    Protocol p;
    p.setType(Protocol::quit);
    p["username"] = username;

    ClientSocketManager::getInstance()->write(p.toByteArray());
}






QString ChatWidget::getUsername() const
{
    return username;
}

void ChatWidget::setUsername(const QString &value)
{
    ui->lbUsername->setText("用户:" + value);
    username = value;
}
