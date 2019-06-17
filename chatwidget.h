#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = 0);
    ~ChatWidget();

    QString getUsername() const;
    void setUsername(const QString &value);

public slots:
    void onChat(QString message);

private slots:
    void on_leChat_returnPressed();
    void onRefreshUser(QStringList list);

private:
    Ui::ChatWidget *ui;
    QString username;

protected:
    virtual void closeEvent(QCloseEvent* event);
};

#endif // CHATWIDGET_H
