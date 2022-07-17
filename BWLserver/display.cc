#include "display.hh"
#include "../includes/bsv.hh"

#include <iostream>
#include <fstream>
#include <cstdlib>

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
#include <signal.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

struct display
{
    int fd;
    drmModeRes *res;
    drmModeConnector *conn;
    uint32_t fbid;
    char *buffer;
    drm_mode_create_dumb create;
} __display;

namespace bwl
{

    void initDisplay(std::string __card_dvc)
    {
        __display.fd = open((std::string("/dev/dri/card") + __card_dvc).c_str(), O_RDWR | O_CLOEXEC); //打开显卡设备文件
        /*获取drm信息*/
        __display.res = drmModeGetResources(__display.fd);
        uint32_t crtc_id = __display.res->crtcs[0];
        uint32_t conn_id = __display.res->connectors[0];
        __display.conn = drmModeGetConnector(__display.fd, conn_id);
        setmonitor(__display.conn->modes[0].hdisplay, __display.conn->modes[0].vdisplay, 4);
        __display.create.width = __display.conn->modes[0].hdisplay;
        __display.create.height = __display.conn->modes[0].vdisplay;
        __display.create.bpp = 32;
        drmIoctl(__display.fd, DRM_IOCTL_MODE_CREATE_DUMB, &__display.create);
        drmModeAddFB(__display.fd, __display.create.width, __display.create.height, 32, 32, __display.create.pitch, __display.create.handle, &__display.fbid);
        drm_mode_map_dumb map;
        map.handle = __display.create.handle;
        drmIoctl(__display.fd, DRM_IOCTL_MODE_MAP_DUMB, &map);
        __display.buffer = (char *)mmap(nullptr, __display.create.size, PROT_READ | PROT_WRITE, MAP_SHARED, __display.fd, map.offset);
        memset(__display.buffer, 0, __display.create.size);
        drmModeSetCrtc(__display.fd, crtc_id, __display.fbid, 0, 0, &conn_id, 1, &__display.conn->modes[0]);
    }

    void destoryDisplay()
    {
        drm_mode_destroy_dumb destroy = {};
        drmModeRmFB(__display.fd, __display.fbid);
        munmap(__display.buffer, __display.create.size);
        destroy.handle = __display.create.handle;
        drmIoctl(__display.fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
        drmModeFreeConnector(__display.conn);
        drmModeFreeResources(__display.res);
        close(__display.fd);
    }

    void *getDrmBuffer()
    {
        return __display.buffer;
    }

    uint64_t getDisplayWidth()
    {
        return __display.create.width;
    }

    uint64_t getDisplayHeight()
    {
        return __display.create.height;
    }
};
