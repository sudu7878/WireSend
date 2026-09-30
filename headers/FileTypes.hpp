#pragma  once

#include <cstdint>
#include <string>
#ifndef FILE_TYPES_HPP
#define FILE_TYPES_HPP

#include "Protocol.hpp"


TemporaryPacketHeader DeserializeHeaderPacket(const std::vector<uint8_t> &hdrbuff);
TemporaryPacketBody DeserializeBodyPacket(const std::vector<uint8_t> &buff, TemporaryPacketHeader &hdr);


/*thef following is the struct to handle the file chunk*/
struct FileChunk{
    std::vector<uint8_t> data;
};

/*note: the variables are named in third person view: "OnTarget" means the peer receiving the file, "OnSrc means the one
sending the file."*/

//there is no seperate packet consturction for ts. i stuff it all into PL_BODY hehehaha
struct FileMetadata{
    uint64_t FileSize;      /*8 BYTES*/
    std::string FileName;   /*dynamic*/
};

struct PendingIncomingFileRequest{
    bool active;
    bool Overwrite;
    std::string FilePathOnTarget;
    FileMetadata metadata;
};  //for the receiving side

struct PendingOutgoingFileRequest{
    bool active = false;
    std::string FilePathOnSrc;
    FileMetadata metadata;
};  //for the sending side


#endif