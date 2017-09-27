cmake_minimum_required(VERSION 2.6)

# Project Configuration
project(Network)
set(CMAKE_CXX_STANDARD 11)

# Source files
set(SOURCE_FILES main.cpp Serveur/Server.cpp Logger/Logger.cpp Session/Session.cpp Session/Message/Message.cpp Session/User.cpp Session/User.h Serveur/Abstract/ISocketAcceptor.h Serveur/Abstract/ISocket.h Serveur/Abstract/BoostAcceptor.cpp Serveur/Abstract/BoostAcceptor.h Serveur/Abstract/BoostSocket.cpp Serveur/Abstract/BoostSocket.h Serveur/Abstract/ISocket.cpp)

include_directories(
./Logger
./Serveur
./Session
)

# Executable declaration
add_executable(
Network
${SOURCE_FILES}
)
target_link_libraries(Network boost_system boost_thread)
