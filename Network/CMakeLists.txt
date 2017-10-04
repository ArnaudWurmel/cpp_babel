cmake_minimum_required(VERSION 2.6)

# Project Configuration
project(Network)
set(CMAKE_CXX_STANDARD 11)
if (!WIN32)
    set(CMAKE_CXX_FLAGS "-g")
endif()

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

# BOOST
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.55.0 REQUIRED COMPONENTS thread regex system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(Network ${Boost_LIBRARIES})
endif()

# Thread
find_package(Threads REQUIRED)
target_link_libraries(Network ${CMAKE_THREAD_LIBS_INIT})

# Windows
if (WIN32)
    macro(get_WIN32_WINNT version)
        if (CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            # Check for Windows 10, b/c we'll need to convert to hex 'A'.
            if ("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif ("${verMajor}" MATCHES "10")
            # Remove all remaining '.' characters.
            string(REPLACE "." "" ver ${ver})
            # Prepend each digit with a zero.
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif(CMAKE_SYSTEM_VERSION)
    endmacro(get_WIN32_WINNT)

    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
endif(WIN32)

if (WIN32)
	target_compile_options(
		Network PRIVATE 
		"/EHsc"
	)
endif(WIN32)