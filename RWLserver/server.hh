#ifndef SERVER_HH
#define SERVER_HH

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h>

#include "../dbg/logs.hh"

struct framebuffer_info
{
    int bufferfile;

    long long size;
    long long width, height;

    char *buffer;
    char **coordination;
} fbinfo;

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

/**
 * @brief 映射缓冲区
 *
 * @param vinfo
 * @param finfo
 */
void mapbuffer(fb_var_screeninfo *vinfo, fb_fix_screeninfo *finfo)
{
    fbinfo.size = vinfo->xres * vinfo->yres * vinfo->bits_per_pixel / 8;
    fbinfo.width = vinfo->xres;
    fbinfo.height = vinfo->yres;

    fbinfo.buffer = (char *)mmap(0, fbinfo.size, PROT_READ | PROT_WRITE, MAP_SHARED, fbinfo.bufferfile, 0);
    fbinfo.coordination = new char *[fbinfo.height];
    for (int i = 0; i < fbinfo.height; i++)
        fbinfo.coordination[i] = fbinfo.buffer + i * finfo->line_length + vinfo->xoffset;
}

#endif
