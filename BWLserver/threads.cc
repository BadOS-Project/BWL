#include "threads.hh"

#include <map>
#include <cstring>

#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include "../includes/bsv.hh"
#include "../includes/syscfg.hh"

extern volatile bool server_running; //服务器运行标志

extern std::map<bwl::id_t, bwl::__page *> pages; //页列表
extern bwl::id_t current_pgid;                   //当前页

extern std::map<bwl::id_t, bwl::__frame *> frames; //窗口列表

namespace bwl
{

    void *drm_buffer; //屏幕缓冲区

    uint8_t updating_signal = 1; //更新标志

    /**
     * @brief 更新函数
     * 
     */
    void updater()
    {
        if (!updateDrmBuffer)
            return;
        else
            updating_signal = 0;
        memcpy(drm_buffer, pages[current_pgid]->server_bg_layer, getBgBuffSize());
    }

    void updateDrmBuffer()
    {
        server::loadCfgFromFs(FPS_CFG);
        useconds_t itv =  interval(server::getFPS());
        while (server_running)
        {
            usleep(itv);
            updater();
        }
    }
};
