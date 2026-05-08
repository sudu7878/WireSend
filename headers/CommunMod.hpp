/*FileName: CommMod.hpp*/

#pragma once
#ifndef COMMUNICATION_MODULE
#define COMMUNICATION_MODULE

#include <stdbool.h>
#include <stdint.h>

#include <sys/errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

#include <ifaddrs.h>


struct Packet{
    uint8_t PL_TYPE;
    uint16_t PL_LEN;
    const char * PL_TXT;
};


#endif  /*COMMINCATION_MODULE*/