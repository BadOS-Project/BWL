#ifndef __bsv_hh_
#define __bsv_hh_

#include "bwl.hh"

namespace bwl
{
    /**
     * @brief 创建页
     * 
     * @param pgid 使用的pgid
     * @return __page* 
     */
    __page *createPage(id_t);
};

#endif
