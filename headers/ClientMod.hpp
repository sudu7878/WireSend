/*FileName: ClientMod.hpp*/

#pragma once
#ifndef CLIENT_MODULE_HPP
#define CLIENT_MODULE_HPP


#include "api.hpp"

#include <stdint.h>


class ClientInstance : public BaseConnectionInstance{
    private:
        uint16_t serv_port;
    public:
        void ConnectToServer(const char* ip, uint16_t port);
        int GetPort();
};

#endif  /*CLIENT_MOD_HPP*/
