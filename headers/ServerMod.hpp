/*FileName: ServerMod.hpp*/

#pragma once
#ifndef SERVER_MODULE_HPP
#define SERVER_MODULE_HPP


#include "api.hpp"
#include <stdint.h>

class ServerInstance : public BaseConnectionInstance{
    private:
        uint16_t serv_port = 8000;
    public:
        int GetPort();
        void StartListening();
        void BindSocketToServer();
        int AcceptClient();
};

void StopServer(ServerInstance& server);

void LockDoor(ServerInstance& server);

#endif  /*SERVER_MOD_HPP*/