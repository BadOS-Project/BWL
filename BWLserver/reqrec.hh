#ifndef __reqrec_hh_
#define __reqrec_hh_

#include "../includes/pipe.hh"
#include "../includes/bsv.hh"
#include <vector>

extern std::vector<bwl::__page *> pages;//页列表
extern bwl::id_t current_pgid;//当前页 

extern std::vector<bwl::__frame *> frames;//窗口列表

extern bwl::Pipe server_recv;

namespace bwl
{
    enum request
    {
        create_frame,
        del_frame,
        create_page,
        del_page,
    };

    /**
     * @brief 请求包
     * 
     * 包括请求类型，
     * 进程号，
     * 请求带有的参数，
     * 魔数(防止其它程序向管道乱发数据，魔数没有对应则扔掉此请求)
     * 
     */
    struct req_pack
    {
        request requests;
        pid_t pid;
        char args[128];
    };
};

void reqrec();

#endif