#include "clientsocketmanager.h"

ClientSocketManager::ClientSocketManager()
    : cs(nullptr)
{

}

ClientSocketManager* ClientSocketManager::instance = new ClientSocketManager;
