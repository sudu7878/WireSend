/*FileName: CmdHandler.cpp*/

#include <string>

#include "CmdHandler.hpp"



Command ParseCommands(const std::string &input){
    if(!input.empty() && input.find("/") == 0){
        if(input == CMD_STOP)       return Command::Stop;
        if(input == CMD_ACCEPT)     return Command::Accept;
        if(input == CMD_FILEPROMPT) return Command::FilePrompt;
        if(input == CMD_REJECT)     return Command::Reject;
        return Command::Unknown;
    }
    return Command::NotACommand;
}