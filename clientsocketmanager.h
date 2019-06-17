#ifndef CLIENTSOCKETMANAGER_H
#define CLIENTSOCKETMANAGER_H

#include "clientsocket.h"
#include <QByteArray>

class ClientSocketManager
{
private:
    ClientSocketManager();
    static ClientSocketManager* instance;
    ClientSocket* cs;

public:
    static ClientSocketManager* getInstance(){
        return instance;
    }

    void initSocket(){
        if(cs != nullptr)   return;
        cs = new ClientSocket;
    }

    const ClientSocket* getClientSocket()const{
        return this->cs;
    }

    inline quint64 write(QByteArray array){
        return cs->write(array);
    }

    inline bool isValid()const{return cs->isValid();}
};

#endif // CLIENTSOCKETMANAGER_H
