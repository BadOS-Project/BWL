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
};

#endif
