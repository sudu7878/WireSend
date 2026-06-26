/*FileName: FileHandler.hpp*/

#pragma once

#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <cstdint>
#include <string>



//there is no seperate packet consturction for ts. i stuff it all into PL_BODY hehehaha
struct FileMetadata{
    uint64_t FileSize;      /*8 BYTES*/
    std::string FileName;   /*dynamic*/
};

struct PendingFileRequest{
    bool active;
    FileMetadata metadata;
};

FileMetadata CreateFileMetadata(std::string &filepath);

bool NegotiateReceiver(int fd, FileMetadata &metadata);
bool AnswerSender(int fd, bool response);


int SendFile(int fd, std::string &SourceFilePath);
int RecvFile(int fd, std::string &DestinationPath);

constexpr uint32_t DEFAULT_FILE_CHUNK_SIZE = 6400;  //64KB


#endif /*FILE_HANDLER_HPP*/