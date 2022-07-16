#define __bwl_server

#include "../includes/bwl.hh"
#include "../includes/pipe.hh"

#include "../dbg/logs.hh"

namespace std
{
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
};

#include <iostream>
#include <fstream>

namespace bwl
{
    namespace bad_wayland_server
    {
        pid_t server_pid;
        uint64_t monitor_size[2];
        int pix_depth;
    };

    void initBadWayland(std::string __monitor_device)
    {
        std::fstream host(BWLDIR + "/host", std::ios::in);
        if (!host.good())
        {
            host.close();
            bwl_exit(-1);
        }
        host >> bad_wayland_server::server_pid;
        host.close();
        std::fstream ff(BWLDIR + "/data/monitor", std::ios::in);
        ff >> bad_wayland_server::monitor_size[0] >> bad_wayland_server::monitor_size[1] >> bad_wayland_server::pix_depth;
    }

    __page *createPage(id_t pgid)
    {
        //发送请求
        req_pack *rp = new req_pack;
        rp->request = requests::create_page;
        rp->pid = std::getpid();
        rp->magic = REQMAGIC;
        rp->arguments.a_create_page.bgspace = colorspace::rgb;
        Pipe push("/dev/bwl/reciever.pipe", bwl::Pipe::out);
        push.write((char *)rp, sizeof(req_pack));
        push.close();
        std::kill(bad_wayland_server::server_pid, SIGBWLREQ);
        delete rp;
        //等待服务器回复
        Pipe getting(BWLDIR + "/reply/" + std::to_string(std::getpid()) + ".pipe", Pipe::in);
        getting.read((char *)&pgid, sizeof(id_t));
        getting.close();
        //获取数据结构
        int shmf = std::shm_open((PAGEDIR + std::to_string(pgid) + "/shm").c_str(), O_RDWR | O_CREAT, 0);
        if (shmf == -1)
        {
            return CREATE_PAGE_FAULT;
        }
        __page *page = (__page *)std::mmap(nullptr, sizeof(__page), PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
        std::close(shmf);
        if (page != (__page *)-1)
        {
            shmf = std::shm_open((PAGEDIR + std::to_string(pgid) + "/bgbuffer").c_str(), O_RDWR | O_CREAT, 0);
            if (shmf == -1)
            {
                return CREATE_PAGE_FAULT;
            }
            page->client_bg_layer = std::mmap(nullptr, getBgBuffSize(), PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
            std::close(shmf);
            if (page->client_bg_layer == (void *)-1)
            {
                std::munmap(page, sizeof(__page));
                std::shm_unlink((PAGEDIR + std::to_string(pgid) + "/shm").c_str());
                return (__page *)MAP_FAILED;
            }
        }

        return page;
    }

    void deletePage(__page *page)
    {
        int pgid = page->pgid;
        std::munmap(page->client_bg_layer, getBgBuffSize());
        std::shm_unlink((PAGEDIR + std::to_string(pgid) + "/bgbuffer").c_str());
        std::munmap(page, sizeof(__page));
        std::shm_unlink((PAGEDIR + std::to_string(pgid) + "/shm").c_str());
        req_pack *rp = new req_pack;
        rp->request = requests::del_page;
        rp->pid = std::getpid();
        rp->magic = REQMAGIC;
        rp->arguments.a_del_page.pgid = pgid;
        Pipe push("/dev/bwl/reciever.pipe", bwl::Pipe::out);
        push.write((char *)rp, sizeof(req_pack));
        push.close();
        std::kill(bad_wayland_server::server_pid, SIGBWLREQ);
        delete rp;
    }

    __frame *createFrame(id_t fid, __page *page, std::string name, uint64_t width, uint64_t height, int64_t x, int64_t y)
    {

        req_pack *rp = new req_pack;
        rp->request = requests::create_frame;
        rp->pid = std::getpid();
        rp->magic = REQMAGIC;
        std::memcpy(rp->arguments.a_create_frame.name, name.c_str(), name.length());
        rp->arguments.a_create_frame.namelen = name.length();
        rp->arguments.a_create_frame.pgid = page->pgid;
        rp->arguments.a_create_frame.pos[0] = x;
        rp->arguments.a_create_frame.pos[1] = y;
        rp->arguments.a_create_frame.size[0] = width;
        rp->arguments.a_create_frame.size[1] = height;
        rp->arguments.a_create_frame.space = colorspace::rgb;
        Pipe push("/dev/bwl/reciever.pipe", bwl::Pipe::out);
        push.write((char *)rp, sizeof(req_pack));
        push.close();
        std::kill(bad_wayland_server::server_pid, SIGBWLREQ);
        delete rp;
        //等待服务器回复
        Pipe getting(BWLDIR + "/reply/" + std::to_string(std::getpid()) + ".pipe", Pipe::in);
        getting.read((char *)&fid, sizeof(id_t));
        getting.close();
        //映射数据
        int shmf = std::shm_open((FRMDIR + std::to_string(fid) + "/shm").c_str(), O_RDWR | O_CREAT, 0);
        if (shmf == -1)
        {
            return CREATE_FRAME_FAULT;
        }
        __frame *frame = (__frame *)std::mmap(nullptr, sizeof(__frame) + name.length() + 1,
                                              PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
        std::close(shmf);

        if (frame != (__frame *)-1)
        {
            shmf = std::shm_open((FRMDIR + std::to_string(fid) + "/buffer").c_str(), O_RDWR | O_EXCL, 0);
            if (shmf == -1)
            {
                return CREATE_FRAME_FAULT;
            }
            frame->client_buf = std::mmap(nullptr, width * height * frame->pixdepth, PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
            std::close(shmf);
            if (frame->client_buf == (void *)-1)
            {
                std::munmap(frame, sizeof(__frame));
                std::shm_unlink((FRMDIR + std::to_string(fid) + "/shm").c_str());
                return (__frame *)MAP_FAILED;
            }
        }
        return frame;
    }

    void deleteFrame(__frame * frame)
    {
        int fid = frame->fid;
        std::munmap(frame->client_buf, frame->size[0] * frame->size[1] * frame->pixdepth);
        std::shm_unlink((FRMDIR + std::to_string(fid) + "/buffer").c_str());
        std::munmap(frame, sizeof(__frame) + frame->namelen);
        std::shm_unlink((FRMDIR + std::to_string(fid) + "/shm").c_str());
        req_pack *rp = new req_pack;
        rp->request = requests::del_frame;
        rp->pid = std::getpid();
        rp->magic = REQMAGIC;
        rp->arguments.a_del_frame.fid = fid;
        Pipe push("/dev/bwl/reciever.pipe", bwl::Pipe::out);
        push.write((char *)rp, sizeof(req_pack));
        push.close();
        std::kill(bad_wayland_server::server_pid, SIGBWLREQ);
        delete rp;
    }

    uint64_t getBgBuffSize()
    {
        return bad_wayland_server::monitor_size[0] * bad_wayland_server::monitor_size[1] * bad_wayland_server::pix_depth;
    }

    uint64_t getMonitorWidth()
    {
        return bad_wayland_server::monitor_size[0];
    }

    uint64_t getMonitorHeight()
    {
        return bad_wayland_server::monitor_size[1];
    }

    int getPixDepth()
    {
        return bad_wayland_server::pix_depth;
    }

};
