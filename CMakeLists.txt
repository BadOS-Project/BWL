cmake_minimum_required(VERSION 3.18)
project(BadWayland)

set(CMAKE_CXX_STANDARD 20)

add_library(bwl SHARED libs/bwl.cc dbg/logs.cc)
add_library(bsv SHARED libs/bsv.cc dbg/logs.cc)
add_library(bpipe SHARED libs/pipe.cc)
add_library(bsyscfg SHARED libs/syscfg.cc)

link_directories(build)

add_executable(bwl-server BWLserver/server.cc dbg/logs.cc BWLserver/reqrec.cc BWLserver/display.cc BWLserver/threads.cc BWLserver/update.cc)
target_link_libraries(bwl-server libbsv.so libbpipe.so librt.so libdrm.so libpthread.so libbsyscfg.so)
