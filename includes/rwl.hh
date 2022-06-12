#ifndef __rwl_hh_
#define __rwl_hh_

#include "types.hh"

namespace rwl
{

    /**
     * @brief 通过进程名获取pid
     *
     * @param task_name
     * @return int
     */
    int get_pid_by_name(const char *task_name);

    class Frame
    {
    private:
        id_t fid;         //窗口id
        pid_t pid;        //进程id
        std::string name; //窗口名

        void *mbuf;       //主缓冲
        int64_t pos[2];   //窗口位置
        uint64_t size[2]; //窗口大小

        void *userbuf;        //用户缓冲
        int64_t ofset[2];     //用户缓冲位置
        uint64_t usersize[2]; //用户缓冲大小

        int pixel_depth;//像素深度

        void __Frame();

        /**
         * @brief 向rwl服务器申请新的缓冲区
         * 
         * @param buff 
         * @param size 
         * @return void* 
         */
        void *realloc_buff(void *buff, size_t size);

    public:
        Frame();
        Frame(std::string name);
        Frame(uint64_t size[2]);
        Frame(int64_t position[2], uint64_t size[2]);
        Frame(std::string name, uint64_t size[2]);
        Frame(std::string name, int64_t position[2], uint64_t size[2]);
        ~Frame();

        void setName(std::string name);
        std::string getName();

        /**
         * 一系列getter函数
         *
         * 这组函数获取的位置是主缓冲的位置
         * 大小是用户缓冲的大小
         */
        int64_t getX();
        int64_t getY();
        int64_t getWidth();
        int64_t getHeight();

        /**
         * @brief 获取主缓冲信息
         *
         * @param buffer 缓冲区
         * @param position 位置
         * @param size 大小
         * @return int 错误码
         */
        int getMainBufferInfo(void *&buffer, int64_t *&position, uint64_t *&size);
        /**
         * @brief 获取用户缓冲信息
         *
         * @param buffer 缓冲区
         * @param position 位置
         * @param size 大小
         * @return int 错误码
         */
        int getUserBufferInfo(void *&buffer, int64_t *&position, uint64_t *&size);

        /**
         * 一系列setter函数
         *
         * 这组函数设置的位置是主缓冲的位置
         * 大小是用户缓冲的大小
         */
        void setX(int64_t x);
        void setY(int64_t y);
        int setWidth(uint64_t width);
        int setHeight(uint64_t height);

        /**
         * @brief 设置主缓冲信息
         *
         * @param buffer 缓冲区
         * @param position 窗口位置
         * @param size 大小
         * @return int 错误码
         */
        int setMainBufferInfo(void *buffer, int64_t position[2], uint64_t size[2]);
        /**
         * @brief 设置用户缓冲信息
         *
         * @param buffer 缓冲区
         * @param position 用户缓冲位置
         * @param size 用户缓冲大小
         * @return int 错误码
         */
        int setUserBufferInfo(void *buffer, int64_t position[2], uint64_t size[2]);

        /**
         * @brief 根据路径移动窗口
         *
         * @param path 路径
         * @param freq 频率
         * @return int 错误码
         */
        int moveByPath(std::vector<int[2]> path, uint64_t freq);

#ifndef __rwl_server
        /**
         * @brief 将窗口附加到rwl显示服务器上
         *
         * @return Frame*
         */
        Frame *attach_to_rwl_server();
#endif

    };

    class Page
    {
    private:
        id_t pgid; //页id

        std::vector<pid_t> owners; //所属进程

        void *layerbg;               //背景层
        std::vector<Frame *> layer0; // 0层
        std::vector<Frame *> layer1; // 1层
    public:
        Page();
        ~Page();

        /**
         * @brief 添加本进程pid至owners
         *
         */
        void addOwner();
        /**
         * @brief 添加窗口
         *
         * @param frame 窗口id
         */
        void addFrame(id_t frame, int layer);
        /**
         * @brief 添加窗口
         *
         * @param frame 窗口
         */
        void addFrame(Frame *frame, int layer);

        /**
         * @brief 将窗口提前
         *
         * @param frame 提前的窗口id
         * @return bool 页中是否存在
         */
        bool pushFrame(id_t frame);
        /**
         * @brief 将窗口提前
         *
         * @param frame 提前的窗口
         * @return bool 页中是否存在
         */
        bool pushFrame(Frame *frame);
    };

};

#endif
