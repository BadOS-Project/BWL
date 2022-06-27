#ifndef __server_hh_
#define __server_hh_

#define __bwl_server

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <signal.h>
#include <libdrm/drm.h>

#include "../dbg/logs.hh"

#define BWLSERVER

#include "../includes/bsv.hh"
#include "../includes/pipe.hh"
#include "reqrec.hh"

//TODO 正式发布时去掉
#define DEBUGGING

std::vector<bwl::__page *> pages;//页列表
bwl::id_t current_pgid;//当前页 

std::vector<bwl::__frame *> frames;//窗口列表

bwl::Pipe server_recv;

/**
 * @brief 切换为守护进程
 */
void switch_to_daemon()
{
    bwl::log("switching to daemon...\nyou can see logs in ~/.bwl/bwl-log and ~/.bwl/bwl-err");
    if(getpwuid(getuid())->pw_name != "root")
    {
        bwl::err("bad wayland must be started with root.");
        bwl::bwl_exit(-3);
    }
    // std::string dir = std::string("/home/") + getpwuid(getuid())->pw_name + "/";
    chdir("/root/");
    mkdir(".bwl", 0755);
    daemon(1, 1);
    freopen("./.bwl/bwl-log", "w", stdout);
    freopen("./.bwl/bwl-err", "w", stderr);
}

void make_bwl_dev()
{
    mkdir("/dev/bwl", 0600);
    std::fstream host("/dev/bwl/host", std::ios::out); //为用户提供bwl服务器的进程号
    host << getpid() << std::endl;
    host.close();
#ifdef DEBUGGING
    std::fstream debug("/dev/bwl/dbg", std::ios::out);
    debug << "Random World Studio" << std::endl;
    debug.close();
#endif
    mkdir("/dev/bwl/pages", 0755);
    mkdir("/dev/bwl/frames", 0755);
    mkdir("/dev/bwl/responds", 0755);
}

/**
 * @brief 启动bwl显示服务器
 */
void start_bwl_server()
{
    server_recv.open("/dev/bwl/reciever.pipe", bwl::Pipe::in);
    signal(SIGBWLREQ, bwl::reqrec);
}

#endif
