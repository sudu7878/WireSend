cmake_minimum_required(VERSION 3.16)

project(Wires VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(headers)

add_executable(wires
    src/CmdHandler.cpp
    src/CommunMod.cpp
    src/api.cpp
    src/ClientMod.cpp
    src/ServerMod.cpp
    src/UserHandler.cpp
    src/FileHandler.cpp
    src/tinyfiledialogs.cpp
)