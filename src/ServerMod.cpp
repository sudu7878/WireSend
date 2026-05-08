/*FileName: ServerMod.cpp*/

#include "ServerMod.hpp"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "UserHandler.hpp"


/*SERVER CLASS FUNCTIONS*/

int ServerInstance::GetPort(){
    return serv_port;
}

void ServerInstance::StartListening(){
    if (listen(sockfd, SOMAXCONN) < 0){
        printf("Error: Couldn't listen.\n");
    }else{
        PrintServerInfo(serv_port);
        printf("Listening on port %d...\n", serv_port);
    }
}

void ServerInstance::BindSocketToServer(){
    struct sockaddr_in serv_address{};
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(serv_port);
    serv_address.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0){
        perror("Binding Error");
        return;
    }
    if(EnableDebug){ printf("Socket bound to 0.0.0.0 at port %d.\n", serv_port);}
}

int ServerInstance::AcceptClient(){
    int ClientCount;
    struct sockaddr_in cli_addr{};
    socklen_t cli_addr_len = sizeof(cli_addr);

    int NewSockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_addr_len);

    if (NewSockfd < 0){
        perror("Unable to accept");
        return -1;
    }
    ClientCount++;
    printf("Client is now connected. Number of clients now: %d\n", ClientCount);
   
    return NewSockfd;
}

void StartServer(){
    ServerInstance NewServer;
        NewServer.CreateSocketFd();
        NewServer.BindSocketToServer();
        NewServer.StartListening();

        while(ProgramRunning){
            
           NewServer.AcceptClient();
           char buff[1024];
           
        }
}