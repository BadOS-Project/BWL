#ifndef __bwl_hh_
#define __bwl_hh_

#include "bsv.hh"

namespace bwl
{
    /**
     * @brief 初始化函数
     * 
     * this function must be the first code in main.
     * however we have not yet wrote codes to search which monitor you're using
     * so you can only write ths function after some monitor getting codes.
     * 
     * 这个函数应该是主函数的第一条语句。
     * 我们现在还没写获取当前显示器的代码，所以这个函数应该只出现在获取当前显示器的代码之后。
     * 
     * @param __monitor_device 
     */
    void initBadWayland(std::string __monitor_device);
};

#endif
