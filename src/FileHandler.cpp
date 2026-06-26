/*FileName: FileHandler.cpp*/

#include "CommunMod.hpp"
#include "api.hpp"

#include <cstdint>
#include <cstdlib>


#include <filesystem>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>

#include "CommunMod.hpp"
#include "FileHandler.hpp"
#include "api.hpp"

namespace fs = std::filesystem;

FileMetadata CreateFileMetadata(std::string &filepath){
    FileMetadata metadata;

    //get the fkn filepath thts =valent to taking in file
    fs::path FilePath(filepath);

    metadata.FileSize = fs::file_size(FilePath);
    metadata.FileName = FilePath.filename();

    return metadata;
}


bool NegotiateReceiver(int fd, FileMetadata &metadata){
    Packet FileQueryPacket;

    std::vector<uint8_t> FileMetadataBuff = SerializeFileMetadataPacket(metadata);

    FileQueryPacket.PL_BODY = FileMetadataBuff;
    FileQueryPacket.PL_TYPE = FILE_NEG;
    FileQueryPacket.PL_CTL = NO_ARG;

    std::vector<uint8_t> PacketBuffer = SerializePacket(FileQueryPacket);
    int SendFlag = SendPacket(PacketBuffer, fd);//sending

    if(SendFlag < 0){
        printf("[FILE HANDLER ERROR] Sending negotiation request failed! Please retry.\n");
        return false;
    } else if (SendFlag == 0){
            if(EnableDebug){printf("[FILE HANDLER] Sent the file negotiation request.\n");};
        ActiveFileNegReq = true; //tells the reciever thread that it shud expect an answer response 
        return true; //symbolizes success
    } else{
            if(EnableDebug){printf("[dbg] [FILE HANDLER ERROR] Send flag returned %d.\n", SendFlag);}
        printf("[FILE HANDLER ERROR] Sending negotiation request failed! Please retry.\n");
        return false;
    }
}

bool AnswerSender(int fd, bool response){
    Packet FileResponsePacket;
    std::string Message;

    if(response == true){
        Message = "Peer accepts the file.\n";
        FileResponsePacket.PL_CTL = FILE_ACCEPT;
    } else {
        Message = "Peer rejected the file.\n";
        FileResponsePacket.PL_CTL = FILE_REJECT;
    }

    FileResponsePacket.PL_BODY.assign(Message.begin(), Message.end());
    FileResponsePacket.PL_TYPE = FILE_NEG;

    std::vector<uint8_t> ResponseBuff = SerializePacket(FileResponsePacket);
    
    int SendFlag = SendPacket(ResponseBuff, fd);//sending

    if(SendFlag < 0){
        printf("[FILE HANDLER ERROR] Sending response failed!\n");
        return false;
    } else if (SendFlag == 0){
            if(EnableDebug){printf("[FILE HANDLER] Answered the sender for file request permission.\n");};
        ActiveFileNegReq = false; //switch off the isolated file negotiation mode. 
        return true; //symbolizes success
    } else{
            if(EnableDebug){printf("[dbg] [FILE HANDLER ERROR] AnswerSender() Send flag returned %d.\n", SendFlag);}
        printf("[FILE HANDLER ERROR] Sending response failed!\n");
        return false;
    }

}