/*FileName: UserHandler.cpp*/

#include "api.hpp"

#include <cstdlib>


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "UserHandler.hpp"


int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Needs arguments.\n");
        printf("Usage for client: ./wires client <ip> <port>\n");
        printf("Usage for server: ./wires server\n");
        printf("Add a -dbg tag to enable debugging.\n");
        return -1;
    }

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-dbg") == 0){
            EnableDebug = true; //enables debugging msgs
            printf("===DEBUG MODE ON: You may see overwhelming info===\n");
        }
    }

    if(strcmp(argv[1], "server") == 0){
        RunningMode = true;
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
    if(RunningMode){StartServer();} 
    //CLIENT MODE
    if (!RunningMode){StartClient(argv[2], atoi(argv[3]));}

    return 0;
}