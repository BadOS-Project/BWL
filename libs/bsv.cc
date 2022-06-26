#include "../includes/bsv.hh"
#include "../dbg/logs.hh"

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>


namespace bwl
{

    uint64_t monitor_size[2];
    int pix_depth;
#define BGBUFSIZE (monitor_size[0] * monitor_size[1] * pix_depth)

    /**
     * @brief 创建页
     *
     * 页结构是共享于bwl服务器和用户程序的
     * 使用posix共享内存实现
     *
     * @param pgid 使用的pgid
     * @return __page*
     */
    __page *createPage(id_t pgid)
    {
        mkdir((PAGEDIR + std::to_string(pgid)).c_str(), 0755);
        int shmf = shm_open((PAGEDIR + std::to_string(pgid) + "shm").c_str(), O_RDWR | O_EXCL, 0);
        if (shmf == -1)
        {
            return CREATE_PAGE_FAULT;
        }
        if (ftruncate(shmf, sizeof(__page)))
        {
            return (__page *)TRUNCATE_SHM_FAULT;
        }
        __page *page = (__page *)mmap(nullptr, sizeof(__page), PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
        close(shmf);

        //如果成功链接共享内存
        //创建背景缓冲
        if (page != (__page *)-1)
        {
            page->pgid = pgid;
            shmf = shm_open((PAGEDIR + std::to_string(pgid) + "bgbuffer").c_str(), O_RDWR | O_EXCL, 0);
            if (shmf == -1)
            {
                return CREATE_PAGE_FAULT;
            }
            if (ftruncate(shmf, BGBUFSIZE))
            {
                return (__page *)TRUNCATE_SHM_FAULT;
            }
            page->server_bg_layer = mmap(nullptr, BGBUFSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
            close(shmf);
            if (page->server_bg_layer == (void *)-1)
            {
                munmap(page, sizeof(__page));
                shm_unlink((PAGEDIR + std::to_string(pgid) + "shm").c_str());
                return (__page *)MAP_FAILED;
            }
        }

        return page;
    }

    void deletePage(__page *page)
    {
        int pgid = page->pgid;
        munmap(page->server_bg_layer, BGBUFSIZE);
        shm_unlink((PAGEDIR + std::to_string(pgid) + "bgbuffer").c_str());
        munmap(page, sizeof(__page));
        shm_unlink((PAGEDIR + std::to_string(pgid) + "shm").c_str());
        rmdir((PAGEDIR + std::to_string(pgid)).c_str());
    }

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
#define FRMBUFSIZE (width * height * pix_depth)
    __frame *createFrame(id_t fid, __page *page, std::string name, int width, int height, int x, int y)
    {
        mkdir((FRMDIR + std::to_string(fid)).c_str(), 0755);
        int shmf = shm_open((FRMDIR + std::to_string(fid) + "shm").c_str(), O_RDWR | O_EXCL, 0);
        if (shmf == -1)
        {
            return CREATE_FRAME_FAULT;
        }
        if (ftruncate(shmf, sizeof(__frame) + name.length()))
        {
            return TRUNCATE_SHM_FAULT;
        }
        __frame *frame = (__frame *)mmap(nullptr, sizeof(__frame) + name.length(),
                                         PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
        close(shmf);

        //如果成功链接共享内存
        //初始化窗口数据，创建缓冲
        if (frame != (__frame *)-1)
        {
            frame->fid = fid;
            frame->pgid = page->pgid;
            frame->namelen = name.length();
            memcpy(frame->name, name.c_str(), name.length());
            frame->de_border_up = 0;
            frame->de_border_down = 0;
            frame->de_border_left = 0;
            frame->de_border_right = 0;
            frame->size[0] = width;
            frame->size[1] = height;
            frame->pos[0] = x;
            frame->pos[1] = y;

            shmf = shm_open((FRMDIR + std::to_string(fid) + "buffer").c_str(), O_RDWR | O_EXCL, 0);
            if (shmf == -1)
            {
                return CREATE_FRAME_FAULT;
            }
            if (ftruncate(shmf, FRMBUFSIZE))
            {
                return TRUNCATE_SHM_FAULT;
            }
            frame->server_buf = mmap(nullptr, FRMBUFSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
            close(shmf);
            if (frame->server_buf == (void *)-1)
            {
                munmap(frame, sizeof(__frame));
                shm_unlink((FRMDIR + std::to_string(fid) + "shm").c_str());
                return (__frame *)MAP_FAILED;
            }
        }
        return frame;
    }

    /**
     * @brief 销毁窗口
     *
     */
    void deleteFrame(__frame *frame)
    {
        int fid = frame->fid;
        munmap(frame->server_buf, frame->size[0] * frame->size[1] * pix_depth);
        shm_unlink((FRMDIR + std::to_string(fid) + "buffer").c_str());
        munmap(frame, sizeof(__frame) + frame->namelen);
        shm_unlink((FRMDIR + std::to_string(fid) + "shm").c_str());
        rmdir((FRMDIR + std::to_string(fid)).c_str());
    }

};
