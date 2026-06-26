/*FileName: CommMod.cpp*/

#include "CommunMod.hpp"
#include "FileHandler.hpp"

#include <cstddef>
#include <cstdint>
#include <stdbool.h>
#include <stdint.h>

#include <sys/errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <cstring>
#include <arpa/inet.h>

#include <ifaddrs.h>

size_t PacketSize = 0;

//the packet is serialized and deserialized in the same order.

template<typename idk>
void WritePacketBuffer(std::vector<uint8_t> &buff, const idk &value){
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
    for(size_t i = 0; i <sizeof(idk); i++){
        buff.push_back(bytes[i]);
    }
}

std::vector<uint8_t> SerializePacket(Packet &data){
    std::vector<uint8_t> PacketBuff;
    
    //1. Data type PL_TYPE
    PacketBuff.push_back(data.PL_TYPE);

    //2. Data length PL_LEN
    data.PL_LEN = data.PL_BODY.size();  /*size of the str*/
    WritePacketBuffer(PacketBuff, htonl(data.PL_LEN));

    //3. Data body PL_BODY
    //filling in the buffer with the body
    for(uint8_t byte : data.PL_BODY){
        PacketBuff.push_back(byte);
    }

    //4. Packet control flag.
    PacketBuff.push_back(data.PL_CTL);
    
    return PacketBuff;
}

TemporaryPacketHeader DeserializeHeaderPacket(const std::vector<uint8_t> &hdrbuff){
    TemporaryPacketHeader RecievedHeaderPacket;
    size_t index = 0;

    RecievedHeaderPacket.type = hdrbuff[index];
        index += sizeof(uint8_t);
        uint32_t Length;

    memcpy(&Length, &hdrbuff[index], sizeof(uint32_t));
    RecievedHeaderPacket.len = ntohl(Length);
    return RecievedHeaderPacket;
}

TemporaryPacketBody DeserializeBodyPacket(const std::vector<uint8_t> &buff, TemporaryPacketHeader &hdr){
    TemporaryPacketBody RecievedBodyPacket;

    size_t index = 0;
    
    for(size_t i = 0; i < hdr.len; i++){
        RecievedBodyPacket.body.push_back(buff[index]);
        index++;
    }

    RecievedBodyPacket.ctl = buff[index];

    return RecievedBodyPacket;
}

Packet CombinePacket(TemporaryPacketHeader &hdr, TemporaryPacketBody &body){
   Packet CombinedPacket;
   CombinedPacket.PL_TYPE = hdr.type;
   CombinedPacket.PL_LEN = hdr.len;
   CombinedPacket.PL_BODY = body.body;
   CombinedPacket.PL_CTL = body.ctl;
   return CombinedPacket;
}  

//File metadat ahandling

std::vector<uint8_t> SerializeFileMetadataPacket(const FileMetadata &meta){
    std::vector<uint8_t> MetadataBuff;

    //1: writing the filesize
    WritePacketBuffer(MetadataBuff, meta.FileSize);
    
    //2: writing the filname size
    uint32_t NameLen = meta.FileName.size();
    WritePacketBuffer(MetadataBuff, NameLen);

    //3: writing the filename
    for (uint8_t byte : meta.FileName){
        MetadataBuff.push_back(byte);
    }

    return MetadataBuff;
}

FileMetadata DeserializeFileMetadataPacket(std::vector<uint8_t> &buff){
    FileMetadata MetadataStruct;
    size_t index = 0;

    uint64_t filesize;
    memcpy(&filesize, &buff[index], sizeof(uint64_t));
        index += sizeof(uint64_t);
    
    uint32_t filenamelen;
    memcpy(&filenamelen, &buff[index], sizeof(uint32_t));
        index += sizeof(uint32_t);
    
    std::string filename;
    for(uint32_t i = 0; i < filenamelen; i++){
        filename.push_back(buff[index]);
        index++;
    }

    MetadataStruct.FileSize = filesize;
    MetadataStruct.FileName = filename;
    
    return MetadataStruct;
}