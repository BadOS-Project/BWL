#define __bwl_server

#include "../includes/bwl.hh"
#include "../includes/pipe.hh"

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

namespace bwl
{

    __page *createPage(id_t pgid)
    {
        //发送请求
        req_pack *rp = new req_pack;
        rp->request = requests::create_page;
        rp->pid = std::getpid();
        rp->magic = REQMAGIC;
        rp->arguments.a_create_page.bgspace = colorspace::rgb;
        //等待服务器回复
        Pipe getting(BWLDIR + "/reply/" + std::to_string(std::getpid()) + ".pipe", Pipe::in);
        getting.read((char *)&pgid, sizeof(id_t));
        getting.close();
        //获取数据结构
        int shmf = std::shm_open((PAGEDIR + std::to_string(pgid) + "shm").c_str(), O_RDWR | O_CREAT, 0);
        if (shmf == -1)
        {
            return CREATE_PAGE_FAULT;
        }
        __page *page = (__page *)std::mmap(nullptr, sizeof(__page), PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
        std::close(shmf);
        if (page != (__page *)-1)
        {
            shmf = std::shm_open((PAGEDIR + std::to_string(pgid) + "bgbuffer").c_str(), O_RDWR | O_CREAT, 0);
            if (shmf == -1)
            {
                return CREATE_PAGE_FAULT;
            }
            page->client_bg_layer = std::mmap(nullptr, getBgBuffSize(), PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);
            std::close(shmf);
            if (page->client_bg_layer == (void *)-1)
            {
                std::munmap(page, sizeof(__page));
                std::shm_unlink((PAGEDIR + std::to_string(pgid) + "shm").c_str());
                return (__page *)MAP_FAILED;
            }
        }

        return page;
    }

};
