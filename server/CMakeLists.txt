cmake_minimum_required(VERSION 2.6)

# Project Configuration
project(cpp_babel C CXX)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_TYPE})

# Files sources
file(
  GLOB_RECURSE
  source_files
  sources/main.cpp
  sources/Server.cpp
  )

include_directories(
  ./headers
  )

# Executable declaration
add_executable(
  server
  ${source_files}
  )
