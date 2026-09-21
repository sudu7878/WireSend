/*FileName: FileHandler.hpp*/

#pragma once

#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <cstdint>
#include <string>
#include "Protocol.hpp"
#include "FileTypes.hpp"


FileMetadata CreateFileMetadata(std::string &filepath);


bool NegotiateReceiver(int fd, FileMetadata &metadata);
bool AnswerSender(int fd, bool response);

int SendFile(int fd, PendingOutgoingFileRequest &OutgoingFile);
int RecvFile(Packet ReceievedFilePacket, PendingIncomingFileRequest &IncomingFile);

constexpr uint32_t DEFAULT_FILE_CHUNK_SIZE = 64 * 1024; //64kb?


#endif /*FILE_HANDLER_HPP*/