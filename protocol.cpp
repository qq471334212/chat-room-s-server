#include "protocol.h"
#include <QByteArray>
#include <QJsonDocument>

/*
 * Json : 网络中，最常见的数据表示方法
 * 通过键值对的方式进行数据的存储。
 * 键值一般为字符串，value值可以是
 * 整形、浮点型、字符串、数组、再套一个json
 *
 * 跨平台，实际传输时，将json序列化之后，再把序列化的内容，传输。
 *
 * 变长包
*/

Protocol::Protocol()
    : type(none), len(0)
{

}

Protocol::Protocol(QByteArray &array)
{
    this->len = *(int*)(array.data());
    this->type = *(Type*)(array.data() + sizeof(int));

    array = array.right(array.size() - headSize());

    this->obj = QJsonDocument::fromJson(array.left(len)).object();
    array = array.right(array.size() - len);
}

QPair<bool, Protocol> Protocol::unpack(QByteArray &array)       //解压包
{
    if(array.size() < headSize()){
        return QPair<bool, Protocol>(false, Protocol());
    }

    int len = *(int*)(array.data());
    if(array.size() < headSize() + len){
        return QPair<bool, Protocol>(false, Protocol());
    }

    return QPair<bool, Protocol>(true, Protocol(array));
}

Protocol::Type Protocol::getType() const
{
    return type;
}

void Protocol::setType(const Type &value)
{
    type = value;
}

QByteArray Protocol::toByteArray()
{
    //包头 + json序列
    //包头：长度 + 类型
    QByteArray array(headSize(), 0);
    QByteArray json = QJsonDocument(obj).toJson();
    this->len = json.size();

    memcpy(array.data(), &this->len, sizeof(int));
    memcpy(array.data() + sizeof(int), &this->type, sizeof(Type));

    return array + json;
}



