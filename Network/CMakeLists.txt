cmake_minimum_required(VERSION 2.6)

# Project Configuration
project(Network)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g")

# Source files
set(SOURCE_FILES main.cpp Serveur/Server.cpp Logger/Logger.cpp Session/Message/Message.cpp Session/User.cpp Session/User.h Serveur/Abstract/ISocketAcceptor.h Serveur/Abstract/ISocket.h Serveur/Abstract/BoostAcceptor.cpp Serveur/Abstract/BoostAcceptor.h Serveur/Abstract/BoostSocket.cpp Serveur/Abstract/BoostSocket.h Serveur/Abstract/ISocket.cpp Session/Channel.cpp Session/Channel.h)

include_directories(
./Logger
./Serveur
./Serveur/Abstract
./Session
./Session/Message
)

# Executable declaration
add_executable(
Network
${SOURCE_FILES}
)
target_link_libraries(Network pthread boost_system boost_thread)
