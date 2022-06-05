cmake_minimum_required(VERSION 3.18)
project(RWLserver)

set(CMAKE_CXX_STANDARD 20)

add_executable(rwl-server RWLserver/server.cc dbg/logs.cc)
