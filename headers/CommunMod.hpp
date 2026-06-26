/*FileName: CommMod.hpp*/

#pragma once
#pragma pack(push, 1)

#include <cstdint>
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
#include <string>
#include <vector>
#include <ifaddrs.h>

#include "FileHandler.hpp"

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


enum PacketType{
    MESSAGE = 0,
    MESSAGE_BROADCAST = 1,

    FILE_TRANSFER = 2,
    FILE_NEG = 3,
    FILE_BEGIN = 4,
    FILE_CHUNK = 5,
    FILE_END = 6
};

enum Flags{
    FILE_SUCC = 7,
    FILE_FAIL = 8,
    CANCEL_TRANS = 9
};

enum ControlType{
    NO_ARG = 10,
    FILE_ACCEPT = 11,
    FILE_REJECT = 12,
    END_CONNECTION = 13
};

/*Never forget to tell this function which endiannes to use or the world is over.*/
template<typename idk>
void WritePacketBuffer(std::vector<uint8_t>& buff, const idk& value); 

/*For normal packets*/
    /*Serialize*/
std::vector<uint8_t> SerializePacket(Packet &data);   
    /*deserialize*/
TemporaryPacketHeader DeserializeHeaderPacket(const std::vector<uint8_t> &hdrbuff);
TemporaryPacketBody DeserializeBodyPacket(const std::vector<uint8_t> &buff, TemporaryPacketHeader &hdr);
Packet CombinePacket(TemporaryPacketHeader &hdr, TemporaryPacketBody &body);

/*For file metadata packets*/
    /*serialize*/
std::vector<uint8_t> SerializeFileMetadataPacket(const FileMetadata &meta);
    /*deserialize*/
FileMetadata DeserializeFileMetadataPacket(std::vector<uint8_t> &buff);

#pragma pack(pop)
#endif  /*COMMINCATION_MODULE*/