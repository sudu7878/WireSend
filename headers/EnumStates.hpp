#pragma once

#include <cstdint>
#ifndef ENUM_STATES_HPP
#define ENUM_STATES_HPP

enum PacketType : std::uint8_t {
    MESSAGE,
    MESSAGE_BROADCAST,
    FILE_TRANSFER,
    FILE_NEG,

    FILE_BEGIN,
    FILE_CHUNK,
    FILE_END,

    FILE_SUCC,
    FILE_FAIL,
    FILE_READY,

    NO_ARG,
    FILE_ACCEPT,
    FILE_REJECT,
    CANCEL_TRANS,
    END_CONNECTION
};


#endif