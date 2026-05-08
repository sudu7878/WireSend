/*FileName: ClientMod.cpp*/

#include "ClientMod.hpp"
#include "api.hpp"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "UserHandler.hpp"

/*CLIENT CLASS FUNCTIONS*/

void ClientInstance::ConnectToServer(const char* ip, uint16_t port){
    struct sockaddr_in serv_address{};
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);
    serv_port = port;
    
    int res = inet_pton(AF_INET, ip, &serv_address.sin_addr);
            
    if(res == 0){
        printf("Invalid IP Address format.\n");
    } else if (res < 0){
        perror("inet_pton");
        return;
    }

    if (connect(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0){
        perror("Connection Error");
        
        return;
    }
            
    printf("Connected to %s:%d.\n", ip, port);
}

int ClientInstance::GetPort(){
    return serv_port;
}


void TerminateConnection(BaseConnectionInstance& ConnectionInstance){
    ConnectionInstance.CloseManually();
}

void StartClient(const char* ip, uint16_t port){
    printf("Attempting to connect server at %s:%d\n", ip, port);
        
    ClientInstance NewClient;
    NewClient.CreateSocketFd();
    NewClient.ConnectToServer(ip, port);

    while(ProgramRunning){
            
    }
}