cmake_minimum_required(VERSION 3.18)
project(BadWayland)

set(CMAKE_CXX_STANDARD 20)

add_library(bwl SHARED libs/bwl.cc)
add_library(bsv SHARED libs/bsv.cc)

add_executable(bwl-server BWLserver/server.cc dbg/logs.cc)
target_link_libraries(bwl-server libbsv.so librt.so libdrm.so)
