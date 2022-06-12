#ifndef __server_hh_
#define __server_hh_

#define __rwl_server

#include <iostream>
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
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <dirent.h>

#include <cstdlib>

#include "../dbg/logs.hh"

/**
 * @brief 切换为守护进程
 */
void switch_to_daemon()
{
    rwl::log("switching to daemon...\nyou can see logs in ~/.rwl/rwl-log and ~/.rwl/rwl-err");
    daemon(1, 1);
    std::string dir = std::string("/home/") + getpwuid(getuid())->pw_name + "/";
    chdir(dir.c_str());
    mkdir(".rwl", 0755);
    freopen("./.rwl/rwl-log", "w", stdout);
    freopen("./.rwl/rwl-err", "w", stderr);
}



/**
 * @brief 启动rwl显示服务器
 */
void start_rwl_server()
{
}

#endif
