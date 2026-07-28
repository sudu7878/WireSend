/*FileName: FileHandler.cpp*/

#include "CommunMod.hpp"
#include "api.hpp"

#include <cstdint>


#include <filesystem>
#include <fstream>
#include <ios>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "CommunMod.hpp"
#include "FileHandler.hpp"
#include "api.hpp"

namespace fs = std::filesystem;

void dbgPrintRecvFileInfo(PendingIncomingFileRequest &IncomingFile){
    std::filesystem::path path(IncomingFile.FilePathOnTarget);
    printf("[dbg] Saving %s to %s.\n", path.filename().stem().string().c_str(), path.c_str());
}

void dbgPrintSendFileInfo(PendingOutgoingFileRequest &OutgoingFile){
    std::filesystem::path path(OutgoingFile.FilePathOnSrc);
    printf("[dbg] Sending %s from %s.\n", path.filename().stem().string().c_str(), path.c_str());
}

FileMetadata CreateFileMetadata(std::string &filepath){
    FileMetadata metadata;

    //get the  filepath thts =valent to taking in file
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

bool CanReceiveFiles(PendingIncomingFileRequest &IncomingFile, FileMetadata meta){
    std::filesystem::path path(IncomingFile.FilePathOnTarget);
    auto info = fs::space(path.parent_path());

    std::string mainName = path.stem().string();    //get the name of the file
    std::string ext = path.extension().string();    //get the extension
    fs::path parent = path.parent_path();           //get the parent folder path to have a target for search

    if(info.available < meta.FileSize){
        printf("[FILE HANLDER MODULE ERROR]: Not enough disk space.\n");
        return false;
    }

    if(fs::exists(path)){
        printf("[WARNING]: The filename already exists. Do you want to overwrite it?\n");
        bool UserResponse = UserAction();

        if (UserResponse){
            IncomingFile.Overwrite = true;  //shud be referenced later
        } else if (!UserResponse){
            printf("[INFO]: A new file will be created.\n");
            int counter = 1;
            fs::path newPath = path;

            /*changing the file name here itself so we update the main struct that gets the name written directly by RecvFile.*/

            while(fs::exists(newPath)){
                newPath = parent / (mainName + "(" + std::to_string(counter) + ")" + ext);
                counter ++;
            }

            IncomingFile.FilePathOnTarget = newPath.string(); //apply the stuff we did
                if(EnableDebug){printf("[dbg] Overwriting action finished.\n");}
        } 
    }
    return true;
}

bool CanSendFiles(PendingOutgoingFileRequest &OutgoingFile, FileMetadata meta){
    std::filesystem::path path(OutgoingFile.FilePathOnSrc);

    std::string mainName = path.stem().string();    //get the name of the file
    std::string ext = path.extension().string();    //get the extension
    fs::path parent = path.parent_path();           //get the parent folder path to have a target for search

    if(!fs::exists(path)){
        return false;
    }
    if(!fs::is_regular_file(path)){
        return false;
    }
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open()){
        printf("[FILE HANDLER MODULER ERROR]: The requested file cannot be opened.\n");
        return false;
    }

    return true;
}

int SendFile(PendingOutgoingFileRequest &OutgoingFile, FileMetadata meta, int fd){
        if(EnableDebug){printf("[dbg] Received command to send files.\n");}
    if(CanSendFiles(OutgoingFile, meta)){
            if(EnableDebug){dbgPrintSendFileInfo(OutgoingFile);}

        std::ifstream file(OutgoingFile.FilePathOnSrc, std::ios::binary);
        std::vector<uint8_t> buffer(DEFAULT_FILE_CHUNK_SIZE);
        
        while(file){
            file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

            std::streamsize bytesRead = file.gcount();

            if(bytesRead <= 0){
                break;
            }
            FileChunk chunk;
            chunk.data.assign(buffer.begin(), buffer.begin() + bytesRead);

            Packet ChunkPacket;
            ChunkPacket.PL_TYPE = FILE_TRANSFER;
            ChunkPacket.PL_CTL = NO_ARG;
            ChunkPacket.PL_BODY = SerializeFileChunk(chunk);

            auto mainBuff = SerializePacket(ChunkPacket);
            int SendStatus = SendPacket(mainBuff, fd);
            if(SendStatus < 0){
                printf("[FILE HANDLER MODULE ERROR]: Sending packet failed.\n");
                return -1;
            }
        }
        return 0;
    } 
    printf("[FILE HANDLER MODULE ERROR]: Cannot send the file braaah.\n");
    return -1;
}

int RecvFile(int fd, FileMetadata meta,PendingIncomingFileRequest &IncomingFile){
            if(EnableDebug){printf("[dbg] Received command to recv files.\n");}

    if(CanReceiveFiles(IncomingFile, meta)){
            if(EnableDebug){dbgPrintRecvFileInfo(IncomingFile);}
        


            
        
    } else {
        printf("[FILE HANDLER ERROR]: Failed to receive the file.\n");
        return -1;
    }

    return 0;
}