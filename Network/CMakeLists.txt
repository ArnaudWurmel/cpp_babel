cmake_minimum_required(VERSION 3.7)
project(Network)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Serveur/Server.cpp Serveur/Server.h Logger/Logger.cpp Logger/Logger.h Session/Session.cpp Session/Session.h Session/Message/Message.cpp Session/Message/Message.h)
add_executable(Network ${SOURCE_FILES})
target_link_libraries(Network boost_system boost_thread)
