/*FileName: UserHandler.hpp*/

#include <cstdint>
#pragma onces
#ifndef USER_HANDLER_HPP
#define USER_HANDLER_HPP

#include <stdint.h>

int StartServer(uint16_t port);
int StartClient(const char* ip, uint16_t port);

#endif /*USER_HANDLER_HPP*/