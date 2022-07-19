#include "update.hh"

#include <cstring>
#include <map>

#include "display.hh"

#include "../includes/bsv.hh"

extern std::map<bwl::id_t, bwl::__page *> pages; //页列表
extern bwl::id_t current_pgid;                   //当前页

extern std::map<bwl::id_t, bwl::__frame *> frames; //窗口列表

namespace bwl
{
    extern void *drm_buffer; //屏幕缓冲区

    extern __updating_signals __ud;

    uint64_t bgbuffsize;

    void update_bg()
    {
        if (__ud.__bg.pos[0] == -1)
        {
            memcpy(drm_buffer, pages[current_pgid]->server_bg_layer, bgbuffsize);
            return;
        }
        for (int i = 0; i < __ud.__bg.size[1]; i++)
        {
            memcpy(((uint32_t *)drm_buffer) + (__ud.__bg.pos[0] + (__ud.__bg.pos[1] + i) * getDisplayWidth()),
                   ((uint32_t *)pages[current_pgid]->server_bg_layer) + (__ud.__bg.pos[0] + (__ud.__bg.pos[1] + i) * getDisplayWidth()),
                   __ud.__bg.size[1] * sizeof(uint32_t));
        }
    }

};
