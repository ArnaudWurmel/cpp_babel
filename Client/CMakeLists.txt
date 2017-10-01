cmake_minimum_required(VERSION 3.5.1)
project(Client)

set(CMAKE_PREFIX_PATH "/opt/Qt5.7.0/5.7/gcc_64/")
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

set(SOURCE_FILES main.cpp Socket/ISocket.h Socket/ISocket.cpp Socket/endpacked.h Socket/packed.h Socket/Message.h Socket/Message.cpp Socket/QtSocket.cpp Socket/QtSocket.h)
add_executable(Client ${SOURCE_FILES})
target_link_libraries(Client Qt5::Widgets Qt5::Network)