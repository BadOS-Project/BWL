#include "threads.hh"

#include <map>
#include <cstring>

#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include "../includes/bsv.hh"

extern std::map<bwl::id_t, bwl::__page *> pages; //页列表
extern bwl::id_t current_pgid;                   //当前页

extern std::map<bwl::id_t, bwl::__frame *> frames; //窗口列表

namespace bwl
{

    void *drm_buffer; //屏幕缓冲区

    void updater(int)
    {
        memcpy(drm_buffer, pages[current_pgid]->server_bg_layer, getBgBuffSize());
    }

    void updateDrmBuffer()
    {
        signal(SIGALRM, updater);
        struct itimerval value;
        value.it_value.tv_sec = 0;
        value.it_value.tv_usec = 500;
        value.it_interval = value.it_value;
        setitimer(ITIMER_REAL, &value, NULL);
    }
};
