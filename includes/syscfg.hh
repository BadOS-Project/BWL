#ifndef __syscfg_hh_
#define __syscfg_hh_

#include <iostream>

namespace bwl
{
    namespace server
    {

#define FPS_CFG 0b1
#define MONISIZE_CFG 0b10
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

        /**
         * @brief 获取显示器大小
         *
         * @param width
         * @param height
         */
        void getMoniSize(uint64_t *width, uint64_t *height);

        /**
         * @brief 设置显示器大小
         * 
         * @param width 
         * @param height 
         */
        void setMoniSize(uint64_t width, uint64_t height);

#define interval(fps) (1000 / fps) //帧率转间隔
    };
};

#endif
