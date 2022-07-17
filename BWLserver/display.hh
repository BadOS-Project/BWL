#ifndef __display_hh_
#define __display_hh_

#include <iostream>

namespace bwl
{

    /**
     * @brief 初始化drm
     *
     * @param __card_dvc 显卡设备
     */
    void initDisplay(std::string __card_dvc);

    /**
     * @brief 关闭drm
     *
     */
    void destoryDisplay();

    /**
     * @brief 获取buffer
     *
     * @return void*
     */
    void *getDrmBuffer();

    /**
     * @brief 获取屏幕宽度
     *
     * @return uint64_t
     */
    uint64_t getDisplayWidth();

    /**
     * @brief 获取屏幕高度
     *
     * @return uint64_t
     */
    uint64_t getDisplayHeight();

#define getDisplayPixDpt() 4
};

#endif
