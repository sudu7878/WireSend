/*FileName: UserHandler.hpp*/

#pragma onces
#ifndef SERVER_MODULE_HPP
#define SERVER_MODULE_HPP

#include <stdint.h>

void StartServer();
void StartClient(const char* ip, uint16_t port);

#endif /*USER_HANDLER_HPP*/