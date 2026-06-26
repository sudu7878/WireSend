/*FileName: api.hpp*/

#pragma once

#include <cstdint>
#ifndef API_MODULE_HPP
#define API_MODULE_HPP


#include <stdbool.h>
#include <stdint.h>

#include <sys/errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <string>

#include <ifaddrs.h>

constexpr auto CMD_STOP = "/~STOP~/";
constexpr auto CMD_FILEPROMPT = "/~FILE~/";
constexpr auto CMD_ACCEPT = "/accept";
constexpr auto CMD_REJECT = "/reject";

extern bool EnableDebug;
extern volatile bool ProgramRunning;
extern bool RunningMode;                       /*1 for server. 0 for client */
extern bool ActiveFileNegReq;
extern bool FileTransferMode;

void HandleExit(int sig);

const char* GetLANIPAddr();

class BaseConnectionInstance{
    protected:
        int sockfd;
    public:
        BaseConnectionInstance();
        bool CreateSocketFd();
        int GetFd();
        virtual ~BaseConnectionInstance() noexcept;

};

void PrintServerInfo(uint16_t port);
void PrintIncomingFileInfo(uint64_t FileSize, std::string FileName);

void TerminateConnection(BaseConnectionInstance& ConnectionInstance);

int RecievePacket(std::vector<uint8_t> &buffer, int fd);

int SendPacket(std::vector<uint8_t> &msgbuff, int fd);

bool UserAction(void);

#endif  /*API_HPP*/