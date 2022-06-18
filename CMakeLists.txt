cmake_minimum_required(VERSION 3.18)
project(BadWayland)

set(CMAKE_CXX_STANDARD 20)

add_executable(bwl-server BWLserver/server.cc dbg/logs.cc)
add_library(bwl SHARED libs/bwl.cc)
