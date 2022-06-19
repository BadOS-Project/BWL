#ifndef __bwl_hh_
#define __bwl_hh_

#include "types.hh"

namespace bwl
{

    const uint32_t MAX_OWNERS = 512;
    const uint32_t MAX_FRAMES = 512;

    struct __frame
    {
        id_t fid;    //窗口id
        pid_t owner; //所属进程
        id_t pgid;   //所在页的id

        uint64_t size[2];
        int64_t pos[2];

        void *server_buf; // bwl混成器使用的缓冲区
        void *client_buf; //用户程序使用的缓冲区

        uint64_t de_border_up;    //桌面环境上边界区
        uint64_t de_border_down;  //桌面环境下边界区
        uint64_t de_border_left;  //桌面环境左边界区
        uint64_t de_border_right; //桌面环境右边界区

        int namelen;  //窗口名长度
        char name[0]; //窗口名
    };

    struct __page
    {
        id_t pgid;                //页id
        pid_t owners[MAX_OWNERS]; //所有所属进程

        void *server_bg_layer;   // bwl使用的背景层缓冲区
        void *client_bg_layer;   //用户程序使用的背景层缓冲区
        id_t layer0[MAX_FRAMES]; // 0层
        int l0c;
        id_t layer1[MAX_FRAMES]; // 1层
        int l1c;
    };

#ifndef BWLSERVER
    int initBWL(); //初始化bwl并且检测bwl服务器
#endif

    /**
     * @brief 创建页
     *
     * @param pgid 对于bwl服务器来说，是使用的pgid
     * @return __page*
     */
#define CREATE_PAGE_FAULT (__page *)(-2)
    __page *createPage(id_t pgid);

    /**
     * @brief 销毁页
     *
     */
    void deletePage(__page *);

    /**
     * @brief 创建窗口
     *
     * @param fid 对于bwl服务器来说，是使用的fid
     * @param name 窗口名
     * @return __frame*
     */
#define CREATE_FRAME_FAULT (__frame *)(-2)
#define TRUNCATE_SHM_FAULT (__frame *)(-3)
    __frame *createFrame(id_t fid, id_t pgid, std::string name, int width, int height, int x, int y);

    /**
     * @brief 销毁窗口
     *
     */
    void deleteFrame(__frame *);

};

#endif
