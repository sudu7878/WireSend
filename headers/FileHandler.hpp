/*FileName: FileHandler.hpp*/

#pragma once

#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <cstdint>
#include <string>


/*note: the variables are named in third person view: "OnTarget" means the peer receiving the file, "OnSrc means the one
sending the file."*/

//there is no seperate packet consturction for ts. i stuff it all into PL_BODY hehehaha
struct FileMetadata{
    uint64_t FileSize;      /*8 BYTES*/
    std::string FileName;   /*dynamic*/
};

struct PendingIncomingFileRequest{
    bool active;
    std::string FilePathOnTarget;
    FileMetadata metadata;
};  //for the receiving side

struct PendingOutgoingFileRequest{
    bool active = false;
    std::string FilePathOnSrc;
    FileMetadata metadata;
};  //for the sending side

FileMetadata CreateFileMetadata(std::string &filepath);

bool NegotiateReceiver(int fd, FileMetadata &metadata);
bool AnswerSender(int fd, bool response);


int SendFile(int fd, FileMetadata meta,PendingOutgoingFileRequest &OutgoingFile);
int RecvFile(int fd, FileMetadata meta, PendingIncomingFileRequest &IncomingFile);

constexpr uint32_t DEFAULT_FILE_CHUNK_SIZE = 6400;  //64KB


#endif /*FILE_HANDLER_HPP*/