/*FileName: ServerMod.hpp*/

#pragma once
#include <cstdint>
#ifndef SERVER_MODULE_HPP
#define SERVER_MODULE_HPP


#include "api.hpp"
#include <stdint.h>

class ServerInstance : public BaseConnectionInstance{
    private:
        uint16_t serv_port;
    public:
        explicit ServerInstance(uint16_t port);
        int GetPort();
        void StartListening();
        bool BindSocketToServer();
        int AcceptClient();

};

void StopServer(ServerInstance& server);

void LockDoor(ServerInstance& server);

#endif  /*SERVER_MOD_HPP*/