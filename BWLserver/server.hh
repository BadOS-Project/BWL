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

#include <cstring>

#include "../dbg/logs.hh"

#define BWLSERVER

#include "../includes/bsv.hh"
#include "../includes/pipe.hh"
#include "reqrec.hh"

#include "display.hh"

// TODO 正式发布时去掉
#define DEBUGGING

#define buffer_at(buffer, x, y, width) ((uint32_t *)buffer)[x + y * width]

std::string monitor_device = "";

std::map<bwl::id_t, bwl::__page *> pages; //页列表
bwl::id_t current_pgid;                   //当前页

std::map<bwl::id_t, bwl::__frame *> frames; //窗口列表

bwl::Pipe server_recv;

/**
 * @brief 切换为守护进程
 */
void switch_to_daemon()
{
    bwl::log("switching to daemon...\nyou can see logs in ~/.bwl/bwl-log and ~/.bwl/bwl-err");
    if (getpwuid(getuid())->pw_name != std::string("root"))
    {
        bwl::err(std::string("bad wayland must be started with root.(") + getpwuid(getuid())->pw_name + " now)");
        bwl::bwl_exit(-3);
    }
    chdir("/root/");
    mkdir(".bwl", 0755);
    daemon(1, 1);
    freopen("./.bwl/bwl-log", "w", stdout);
    freopen("./.bwl/bwl-err", "w", stderr);
}

void make_bwl_dev()
{
    mkdir(BWLDIR.c_str(), 0755);
    mkdir((BWLDIR + "/data").c_str(), 0755);
    std::fstream host(BWLDIR + "/host", std::ios::out); //为用户提供bwl服务器的进程号
    host << getpid() << std::endl;
    host.close();
#ifdef DEBUGGING
    std::fstream debug(BWLDIR + "/dbg", std::ios::out);
    debug << "Random World Studio" << std::endl;
    debug.close();
#endif
    mkdir((BWLDIR + "/pages").c_str(), 0755);
    mkdir((BWLDIR + "/frames").c_str(), 0755);
    mkdir((BWLDIR + "/reply").c_str(), 0755);
}

/**
 * @brief 启动bwl显示服务器
 */
void start_bwl_server()
{
    server_recv.open("/dev/bwl/reciever.pipe", bwl::Pipe::in);
    signal(SIGBWLREQ, bwl::reqrec);
    bwl::initDisplay(monitor_device);
    char *buffer = (char *)bwl::getDrmBuffer();
    std::fstream data(BWLDIR + "/data/monitor", std::ios::out);
    data << bwl::getDisplayWidth() << '\n'
         << bwl::getDisplayHeight() << '\n'
         << getDisplayPixDpt();
    data.close();
    int w = bwl::getDisplayWidth();
    int h = bwl::getDisplayHeight();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h / 3; j++)
        {
            buffer_at(buffer, i, j, w) = (0xff * i / w) << 16;
        }
        for (int j = h / 3; j < 2 * h / 3; j++)
        {
            buffer_at(buffer, i, j, w) = (0xff * i / w) << 8;
        }
        for (int j = 2 * h / 3; j < h; j++)
        {
            buffer_at(buffer, i, j, w) = (0xff * i / w) << 0;
        }
    }
}

#endif
