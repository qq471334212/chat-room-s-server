#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "protocol.h"
#include "clientsocketmanager.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_btnRegister_clicked()
{
    QString username = ui->leUsername->text();
    QString userpwd = ui->leUserpwd->text();

    ui->leUsername->clear();
    ui->leUserpwd->clear();

    Protocol p;
    p.setType(Protocol::regist);
    p["username"] = username;
    p["userpwd"] = userpwd;

    ClientSocketManager::getInstance()->write(p.toByteArray());
}

void LoginWidget::on_btnLogin_clicked()
{
    QString username = ui->leUsername->text();
    QString userpwd = ui->leUserpwd->text();

    ui->leUsername->clear();
    ui->leUserpwd->clear();

    Protocol p;
    p.setType(Protocol::login);
    p["username"] = username;
    p["userpwd"] = userpwd;

    ClientSocketManager::getInstance()->write(p.toByteArray());
}
