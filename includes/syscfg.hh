#ifndef __syscfg_hh_
#define __syscfg_hh_

#include <iostream>

namespace bwl
{
    namespace server
    {

#define FPS_CFG 0b1
        /**
         * @brief 从文件读取配置
         * 
         * @param opt 
         */
        void loadCfgFromFs(uint64_t opt);

        /**
         * @brief 获取系统帧率
         *
         * @return uint32_t
         */
        _Float64x getFPS();

#define interval(fps) (1000 / fps) //帧率转间隔
    };
};

#endif
