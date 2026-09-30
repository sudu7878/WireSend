#pragma once

#include <cstdint>
#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <stdbool.h>
#include <stdint.h>

#include <sys/errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <ifaddrs.h>


/*this is the packet to send*/
struct Packet{
    uint8_t PL_TYPE;                    /*1 BYTE*/
    uint32_t PL_LEN;                    /*4 BYTE*/
    std::vector<uint8_t> PL_BODY;       /*dynamic*/
    uint8_t PL_CTL;                     /*1 BYTE*/
};

/*the following the packet to recieve*/
struct TemporaryPacketHeader{
    uint8_t type;
    uint32_t len;
};
struct TemporaryPacketBody{
    std::vector<uint8_t> body;
    uint8_t ctl;
};


#endif