/*FileName: ServerMod.cpp*/

#include "ServerMod.hpp"
#include "CommunMod.hpp"
#include "FileHandler.hpp"
#include "UserHandler.hpp"
#include "api.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include <iostream>

#include <thread>


/*SERVER CLASS FUNCTIONS*/

int CommunicationSocketFd = 0;
bool ServerConnected = false;
PendingFileRequest IncomingFileRequest;


int ServerInstance::GetPort(){
    return serv_port;
}

void ServerInstance::StartListening(){
    if (listen(sockfd, 5) < 0){
        printf("Error: Couldn't listen.\n");
    }else{
        PrintServerInfo(serv_port);
        printf("Listening on port %d...\n", serv_port);
    }
}

bool ServerInstance::BindSocketToServer(){
    struct sockaddr_in serv_address{};
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(serv_port);
    serv_address.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0){
        perror("[ERROR] Binding Error");
        return false;
    }
        if(EnableDebug){ printf("[dbg] Socket bound to port %d.\n", serv_port);}
    return true;
}

int ClientCount = 0;

int ServerInstance::AcceptClient(){
  
    struct sockaddr_in cli_addr{};
    socklen_t cli_addr_len = sizeof(cli_addr);

    int NewSockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_addr_len);

        if(EnableDebug){printf("[dbg] Server accepted connection. Listening socket FD: %d.\n", NewSockfd);}

    if (NewSockfd < 0){
            if(EnableDebug){printf("[dbg] Server failed to accept client. Listening socket FD: %d.\n", NewSockfd);}
        perror("[ERROR] Unable to accept client");
        return -1;
    }
    ClientCount++;
    printf("Client is now connected. Number of clients now: %d\n", ClientCount);

    ServerConnected = true;

    CommunicationSocketFd = NewSockfd;
   
    return NewSockfd;
}

void BroadcastServerMsg(const std::string& message){
    Packet BroadcastPacket;

    BroadcastPacket.PL_BODY.assign(message.begin(), message.end());
    BroadcastPacket.PL_TYPE = MESSAGE_BROADCAST;
    
    std::vector<uint8_t> BroadcastBuffer = SerializePacket(BroadcastPacket);

    int SendFlag = SendPacket(BroadcastBuffer, CommunicationSocketFd);

    if (SendFlag < 0){
        perror("[CRITICAL SERVER ERROR] Could not write into server message buffer");
            if(EnableDebug){printf("[dbg] Broadcast FAIL.\n");}
    };
        if(EnableDebug){printf("[dbg] Broadcast successful.\n");}
}

int RunRecvThread(ServerInstance& server){
        //if(EnableDebug){printf("[dbg] Recieve thread for server is running. Server connected = %d\n", ServerConnected);}
    while(ServerConnected == true){
        //if(EnableDebug){printf("[dbg] INSIDE THE RECV LOOP NOW.\n");}

        /*LOGIC FOR RECIEVING PACKET*/    

        Packet MessagePacket;

         /*to recieve header packet*/
        std::vector<uint8_t> RecvMsgHdrBuff(5);
        TemporaryPacketHeader HeaderPacket;

        int RecieveStatusHdr = RecievePacket(RecvMsgHdrBuff, CommunicationSocketFd);
        if (RecieveStatusHdr == 2){
            ClientCount--;
            ServerConnected = false;
            break;
        } else if(RecieveStatusHdr < 0){
                if(EnableDebug){printf("[dbg] Reading incoming buffer failed.\n");}
            perror("[ERROR] Receiving packet failed");
            return -1;
        }
        
         /*to recieve body packet based on the header packet*/

        
        HeaderPacket = DeserializeHeaderPacket(RecvMsgHdrBuff);
            if(EnableDebug){printf("[dbg] Header Type: %d | Header Len: %u\n", HeaderPacket.type, HeaderPacket.len);}

        if(HeaderPacket.type == FILE_NEG && EnableDebug && ActiveFileNegReq){
            printf("[dbg] A file transfer negotiation request detected.\n");
        }

        TemporaryPacketBody BodyPacket;
        
        size_t BytesToRecieve = HeaderPacket.len + sizeof(uint8_t);

        std::vector<uint8_t> RecvMsgBodyBuff(BytesToRecieve);

        int RecieveStatusBdy = RecievePacket(RecvMsgBodyBuff, CommunicationSocketFd);
        if (RecieveStatusBdy == 2){
                if(EnableDebug){printf("[dbg] Decrementing client.\n");}
            ClientCount--;
            ServerConnected = false;
            break;
        } else if(RecieveStatusBdy < 0){
            perror("[ERROR] Receiving packet failed\n");
            break;
        }

        BodyPacket = DeserializeBodyPacket(RecvMsgBodyBuff, HeaderPacket);

        /*Combining packet based on the Header and the Body packet*/
    
        MessagePacket = CombinePacket(HeaderPacket, BodyPacket);

        if (MessagePacket.PL_TYPE == MESSAGE){
            //CONVERT BINARY TO ASCII
            std::string ReceivedText(MessagePacket.PL_BODY.begin(), MessagePacket.PL_BODY.end());

            printf("[CLIENT]: %s\n", ReceivedText.c_str());

            /*PARSING LOGIC*/
            if (MessagePacket.PL_TYPE == MESSAGE_BROADCAST){
                printf("[BROADCAST] Server: %s", ReceivedText.c_str());
            }

        /*for the case where we are the " first time listeners" and are expected to ANSWER the peer.*/
        } else if (MessagePacket.PL_TYPE == FILE_NEG && MessagePacket.PL_CTL == NO_ARG && !ActiveFileNegReq){
            FileMetadata metadata = DeserializeFileMetadataPacket(MessagePacket.PL_BODY);
            IncomingFileRequest.active = true;
            IncomingFileRequest.metadata = metadata;
            PrintIncomingFileInfo(metadata.FileSize, metadata.FileName);
        
        /*for the case where we are the "askers" and we expect a RESPONSE from the peer.*/

        } else if (MessagePacket.PL_TYPE == FILE_NEG && MessagePacket.PL_CTL == FILE_ACCEPT && ActiveFileNegReq == true){
            printf("YAY USER ACCEPTED THE FILE!! TRANSFER WILL BEGIN HERE.\n");
            //TODO: implement the actual file transfer.
        } else if (MessagePacket.PL_TYPE == FILE_NEG && MessagePacket.PL_CTL == FILE_REJECT && ActiveFileNegReq == true){
            printf("[INFO] The peer rejected to receive file(s).\n");
        }

        switch (MessagePacket.PL_CTL) {
            case NO_ARG:
                break;
            case END_CONNECTION:
                printf("[ACTION] CLIENT LEFT.\n");
                break;
            case CANCEL_TRANS:
                if(MessagePacket.PL_TYPE == FILE_NEG){
                    IncomingFileRequest.active = false;
                    printf("[INFO] Sorry, file transfer request was cancelled by the sender.\n");
                }
                
                //TODO: add a deleting the half-transferred filed logic here (add a file-transfer detection logic 1st)
        }
    }   
    return 0;
}

void StopServer(ServerInstance& server){
    BroadcastServerMsg("Shutting down the server!\n");
    ServerConnected = false;
    shutdown(CommunicationSocketFd, SHUT_WR);
        if(EnableDebug){printf("[dbg] Called shutdown()\n");}
    close(server.GetFd());
        if(EnableDebug){printf("[dbg] Server connection socket closure successful.\n");}
    printf("[SERVER] Stopped the server.\n");
}

void LockDoor(ServerInstance& server){
     close(server.GetFd());
            if(EnableDebug){printf("[dbg] Server connection socket closure successful.\n");}
}

//SERVER LOOP

int StartServer(){

    /*INITIALIZE SERVER*/
    ServerInstance NewServer;
        NewServer.CreateSocketFd();
            if(EnableDebug){printf("[dbg] Server socket creation successful. Socket FD: %d\n", NewServer.GetFd());}
        bool BindFlag = NewServer.BindSocketToServer();

        //necessary err check
        if(BindFlag == false){
                if(EnableDebug){printf("[dbg] Binding socekt to address FAILED.\n");}
            StopServer(NewServer);
            ProgramRunning = false;
            return -1; 
        }
        
        if(ClientCount < 1){
            NewServer.StartListening();
            if(EnableDebug){printf("[dbg] Server reached the listening state successfully.\n");}
            NewServer.AcceptClient();
                if(EnableDebug){printf("[dbg] Server accepted a client.\n");}
            ServerConnected = true;
        } else {
            printf("[SERVER] Max number of clients reached. Couldn't connect more devices.\n");
        }

        std::thread RecvThread(RunRecvThread, std::ref(NewServer));
      
        BroadcastServerMsg("Accepted a client!\n");
                if(EnableDebug){printf("[dbg] Broadcast successful.\n");}


        /*MAIN LOOP*/
        while(ProgramRunning){
           Packet MessagePacket;
           MessagePacket.PL_TYPE = MESSAGE;
           MessagePacket.PL_CTL = NO_ARG;

           std::string InputText;
            
            std::getline(std::cin, InputText);
            MessagePacket.PL_BODY.assign(InputText.begin(), InputText.end());

            if(InputText== "/~STOP~/"){
                StopServer(NewServer);
                if(EnableDebug){printf("[dbg] Recieved string to stop the server.\n");}

            /*if user wants to accept file (note that recv thread shud set IncomingFileRequest.active = true)*/
            } else if (InputText == "/accept"){
               if(IncomingFileRequest.active == true){
                    if(EnableDebug){printf("[dbg] User accepts the file.\n");}
                AnswerSender(CommunicationSocketFd, true);
               } else {
                printf("[INFO] There are no pending file transfer requets to accept.\n");
               }
               continue;

            /*if user wants to reject file (note that recv thread shud set IncomingFileRequest.active = true)*/
            } else if (InputText == "/reject"){
               if(IncomingFileRequest.active == true){
                    if(EnableDebug){printf("[dbg] User rejects the file.\n");}
                AnswerSender(CommunicationSocketFd, false);
               } else {
                printf("[INFO] There are no pending file transfer requets to reject.\n");
               }
               continue;
            } 

            /*SENDING PACKET LOGIC*/
          
           std::vector<uint8_t> MessageBuffer = SerializePacket(MessagePacket);
                //if(EnableDebug){printf("[dbg] Made the packet ready for sending... calling send() now.\n");}


            int SendFlag =  SendPacket(MessageBuffer, CommunicationSocketFd);
            
            if(SendFlag == 0){
                    if(EnableDebug){printf("[dbg] Packet sending successful.\n");}
            } else if (SendFlag < 0){
                    break;
            }

            printf("[YOU]: %s\n", InputText.c_str());
           
        }

        ServerConnected  = false;
        StopServer(NewServer);

        return 0;
}