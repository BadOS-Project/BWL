#include "threads.hh"

#include <map>
#include <cstring>

#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include "update.hh"

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

    extern uint64_t bgbuffsize;

    __updating_signals __ud;

    /**
     * @brief 更新函数
     *
     */
    void updater()
    {
        if (!updating_signal) //没有更新屏幕的需求就直接返回
            return;
        else
            updating_signal = 0;
        //检查背景更新
        if(__ud.__bg.sig)
        {
            update_bg();
            __ud.__bg.sig = 0;
        }
    }

    void updateDrmBuffer()
    {
        bgbuffsize = getBgBuffSize();
        server::loadCfgFromFs(FPS_CFG);              //加载帧率配置
        useconds_t itv = interval(server::getFPS()); //获取帧率并换算为每帧时间间隔
        while (server_running)
        {
            usleep(itv); //睡眠
            updater();   //更新屏幕
        }
    }
};
