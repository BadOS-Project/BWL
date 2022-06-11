#ifndef __server_hh_
#define __server_hh_

#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>


#include "../dbg/logs.hh"


/**
 * @brief 切换为守护进程
 *
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

#endif
