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
#include <thread>

#include "../dbg/logs.hh"

#define BWLSERVER

#include "../includes/bsv.hh"
#include "../includes/pipe.hh"
#include "reqrec.hh"

#include "display.hh"
#include "threads.hh"

// TODO 正式发布时去掉
#define DEBUGGING

#define buffer_at(buffer, x, y, width) ((uint32_t *)buffer)[x + y * width]

std::string monitor_device = "";

bool server_running; //服务器运行标志

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

/**
 * @brief 创建bad wayland设备目录
 * 
 */
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

std::thread *update_drm_buffer;

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
    bwl::setmonitor(bwl::getDisplayWidth(), bwl::getDisplayHeight(), getDisplayPixDpt());
    bwl::drm_buffer = buffer;
    bwl::log("init finished.");
    //初始化完成
    //初始化数据
    bwl::__page *p0 = bwl::createPage(0);
    if (p0 == (bwl::__page *)-2)
    {
        bwl::bwl_exit(30);
    }
    pages[0] = p0;
    current_pgid = 0;
    pages[0]->owners[pages[0]->ownerc++] = getpid();
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
        {
            buffer_at(p0->server_bg_layer, i, j, bwl::getDisplayWidth()) = 0x00ffffff;
        }
    server_running = true;
    //启动线程
    update_drm_buffer = new std::thread(bwl::updateDrmBuffer);
    bwl::log("first print");
}

#endif
