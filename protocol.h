#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QJsonObject>
#include <QPair>

class Protocol
{
public:
    enum Type{
        none,
        regist,
        login,
        quit,
        chat,
        refreshUser     //刷新用户
    };

    Protocol();
    Protocol(QByteArray& array);

    static QPair<bool, Protocol> unpack(QByteArray& array);

    QJsonValue operator[](const QString& key)const{
        return obj[key];
    }
    QJsonValueRef operator[](const QString& key){
        return obj[key];
    }


    Type getType() const;
    void setType(const Type &value);

    QByteArray toByteArray();
    static inline int headSize(){
        return sizeof(int) + sizeof(Type);
    }

private:
    Type type;
    int len;

    QJsonObject obj;
};

#endif // PROTOCOL_H
