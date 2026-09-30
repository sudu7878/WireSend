/*FileName: CmdHandler.hpp*/

#pragma once
#ifndef CMD_HANDLER_HPP
#define CMD_HANDLER_HPP

#include <string>


constexpr auto CMD_STOP = "/stop";
constexpr auto CMD_FILEPROMPT = "/f";
constexpr auto CMD_ACCEPT = "/a";
constexpr auto CMD_REJECT = "/r";

enum class Command{
    Stop,
    Accept, 
    Reject, 
    FilePrompt, 
    Unknown,
    NotACommand
};

Command ParseCommands(const std::string &input);

#endif  /*CMD_HANDLER_HPP*/
