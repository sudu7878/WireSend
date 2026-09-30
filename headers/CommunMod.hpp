/*FileName: CommMod.hpp*/

#pragma once

#include <cstdint>
#ifndef COMMUNICATION_MODULE_HPP
#define COMMUNICATION_MODULE_HPP

#include <stdbool.h>
#include <stdint.h>

#include "Protocol.hpp"
#include "FileTypes.hpp"


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


/*Never forget to tell this function which endiannes to use or the world is over.*/
template<typename idk>
void WritePacketBuffer(std::vector<uint8_t>& buff, const idk& value); 

/*For normal packets*/
    /*Serialize*/
std::vector<uint8_t> SerializePacket(Packet &data);   
    /*deserialize*/

Packet CombinePacket(TemporaryPacketHeader &hdr, TemporaryPacketBody &body);

/*For file metadata packets*/
    /*serialize*/
std::vector<uint8_t> SerializeFileMetadataPacket(const FileMetadata &meta);
    /*deserialize*/
FileMetadata DeserializeFileMetadataPacket(std::vector<uint8_t> &buff);

/*For file chunk packets*/
    /*serialize*/
std::vector<uint8_t> SerializeFileChunk(const FileChunk& chunk);
    /*deserialize*/
FileChunk DeserializeFileChunk(const std::vector<uint8_t>& buff);

#endif  /*COMMINCATION_MODULE*/