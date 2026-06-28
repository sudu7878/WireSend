/*FileName: UserHandler.cpp*/

#include "api.hpp"

#include <cstdint>
#include <cstdlib>


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <charconv>
#include <cstring>

#include "UserHandler.hpp"

bool ParsePort(const char* text, uint16_t& port){
    uint32_t ParsedPort = 0;
    const char* end = text + std::strlen(text);
    auto result = std::from_chars(text, end, ParsedPort);

    if(result.ec != std::errc{} || result.ptr != end){
        return false;
    }

    if(ParsedPort < 1024 || ParsedPort > 65535){
        return false;
    }

    port = static_cast<uint16_t>(ParsedPort);
    return true;
}

uint16_t ServerPort;

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Needs arguments.\n");
        printf("Usage for client: ./wires client <server-ip> <server-port>\n");
        printf("Usage for server: ./wires server <port>\n");
        printf("Add a -dbg tag to enable debugging.\n");
        return -1;
    }

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-dbg") == 0){
            EnableDebug = true;
            printf("===DEBUG MODE ON: You may see overwhelming info===\n");
        }
    }

    if(strcmp(argv[1], "server") == 0){
        RunningMode = true;
        if(argc < 3){
            printf("Usage: ./wires server <port>\n");
            return -1;
        }

        if(!ParsePort(argv[2], ServerPort)){
            printf("[ERROR]: Invalid port. Choose a port between 1024 and 65535.\n");
            return -1;
        }
        printf("WireSend running in server mode.\n");

    } else if(strcmp(argv[1], "client") == 0){
        RunningMode = false;
        if(argc < 4){
            printf("Usage: ./wires client <ip> <port>\n");
            return -1;
        }
        printf("WireSend running in client mode.\n");

    } else {
        printf("Invalid arguments.\n");
        printf("Usage for client: ./wires client <ip> <port>\n");
        printf("Usage for server: ./wires server\n");
        printf("Add a -dbg tag to enable debugging.\n");
        return -1;
    }
    
    //SERVER MODE
    if(RunningMode){StartServer(ServerPort);} 
    //CLIENT MODE
    if (!RunningMode){StartClient(argv[2], atoi(argv[3]));}

    return 0;
}