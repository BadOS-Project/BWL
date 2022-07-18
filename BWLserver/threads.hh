#ifndef __threads_hh_
#define __threads_hh_

#include "../includes/types.hh"

namespace bwl
{
    extern void *drm_buffer; //屏幕缓冲区

    extern uint8_t updating_signal;

    /**
     * @brief 更新缓冲区线程
     *
     */
    void updateDrmBuffer();

};

#endif
