#ifndef __reqrec_hh_
#define __reqrec_hh_

#include "../includes/pipe.hh"
#include "../includes/bsv.hh"
#include <vector>

namespace bwl
{
    enum request
    {
        create_page,  //创建页
        del_page,     //删除页
        create_frame, //创建窗口
        del_frame,    //删除窗口
        switch_as,    //切换顶层窗口
        wakeup,       //唤醒使服务器刷新屏幕
    };

#define REQMAGIC 0xa68c5cce7be74433

#define SIGBWLREQ 38

    struct arg_req_creafra
    {
        id_t pgid;
        uint64_t size[2];
        int64_t pos[2];
        int namelen;
        char name[128];
        colorspace space;
    };

    struct arg_req_delfra
    {
        id_t fid;
    };

    struct arg_req_creapg
    {
        colorspace bgspace;
    };

    struct arg_req_delpg
    {
        id_t pgid;
    };

    struct arg_req_switch
    {
        id_t pgid;
        id_t fid;
    };

    union arg_req_t
    {
        arg_req_creafra a_create_frame;
        arg_req_delfra a_del_frame;
        arg_req_creapg a_create_page;
        arg_req_delpg a_del_page;
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
        request request;
        pid_t pid;
        uint64_t magic;
        arg_req_t arguments;
    };

    /**
     * @brief req信号接受函数
     *
     */
    void reqrec(int);

};

#endif