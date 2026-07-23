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

/*thef following is the struct to store the file chunk*/
struct FileChunk{
    uint32_t ChunkSize;
    std::vector<uint8_t> data;
};


enum PacketType{
    MESSAGE,
    MESSAGE_BROADCAST,
    FILE_TRANSFER,
    FILE_NEG,
    FILE_BEGIN,
    FILE_CHUNK,
    FILE_END
};

enum Flags{
    FILE_SUCC,
    FILE_FAIL,
    FILE_READY,
};

enum ControlType{
    NO_ARG,
    FILE_ACCEPT,
    FILE_REJECT,
    CANCEL_TRANS,
    END_CONNECTION
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