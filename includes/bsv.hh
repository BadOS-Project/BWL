#ifndef __bsv_hh_
#define __bsv_hh_

#include "types.hh"

namespace bwl
{

    /* wayland base data structure
        基础数据结构
    */

    const uint32_t MAX_OWNERS = 512;
    const uint32_t MAX_FRAMES = 512;

    enum colorspace //色彩空间
    {
        rgb,
        hsv,
        cmyk,
    };

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

        colorspace space; //缓冲区使用的色彩空间

        bool refresh;       //刷新标志
        int64_t refrloc[2]; //刷新区域位置
        uint64_t refrsz[2]; //刷新区域大小

        int pixdepth;//像素深度

        int namelen;  //窗口名长度
        char name[0]; //窗口名
    };

    struct __page
    {
        id_t pgid;                //页id
        pid_t owners[MAX_OWNERS]; //所有所属进程

        void *server_bg_layer; // bwl使用的背景层缓冲区
        void *client_bg_layer; //用户程序使用的背景层缓冲区
        colorspace space;      //背景颜色空间

        id_t layer0[MAX_FRAMES]; // 0层
        int l0c;                 // layer 0 counter
        id_t layer1[MAX_FRAMES]; // 1层
        int l1c;                 // layer 0 counter
    };

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
     * @param fid 窗口id
     * @param page 所在页
     * @param name 窗口名
     * @param width 宽度
     * @param height 高度
     * @param x 横坐标
     * @param y 纵坐标
     * @return __frame*
     */
#define CREATE_FRAME_FAULT (__frame *)(-2)
#define TRUNCATE_SHM_FAULT (__frame *)(-3)
    __frame *createFrame(id_t fid, __page *page, std::string name, uint64_t width, uint64_t height, int64_t x, int64_t y);

    /**
     * @brief 销毁窗口
     *
     */
    void deleteFrame(__frame *);

    /**
     * @brief 设置显示器参数
     *
     * @param width
     * @param height
     * @param pix_depth
     */
    void setmonitor(int width, int height, int pix_depth);

    /**
     * @brief 获取背景缓冲大小
     *
     * @return uint64_t
     */
    uint64_t getBgBuffSize(); // TODO 未实现

    /* client-server exchange
        用户-服务器信息交流
    */

    enum requests
    {
        create_page,  //创建页
        del_page,     //删除页
        create_frame, //创建窗口
        del_frame,    //删除窗口
        switch_as,    //切换顶层窗口
        wakeup,       //唤醒使服务器刷新屏幕
    };

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
        requests request;
        pid_t pid;
        uint64_t magic;
        arg_req_t arguments;
    };

};

#endif
