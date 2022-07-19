#ifndef __update_hh_
#define __update_hh_

#include <iostream>

namespace bwl
{
    struct __updating_signals
    {
        struct bg
        {
            uint8_t sig;      //更新标志
            int64_t pos[2];   //更新位置
            uint64_t size[2]; //更新大小
        } __bg;
    };

    /**
     * @brief 更新背景
     * 
     */
    void update_bg();

};

#endif
