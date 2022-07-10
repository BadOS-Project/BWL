#include "../includes/pipe.hh"
namespace std
{
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
};

#include <cstdio>

namespace bwl
{

#define BPIPE_MAGIC 0x8cffa60091dd00c4
#define BPIPE_MAXSIZE 0x1000000

    Pipe::Pipe()
    {
    }

    Pipe::Pipe(std::string path, int mode)
    {
        name = path;
        open(path, mode);
    }

    Pipe::~Pipe()
    {
        std::close(fd);
    }

    int Pipe::read(char *buffer, int size)
    {
        if (!size)
            return 0;
        if (mode == in)
        {
            std::read(fd, buffer, size);
            bool wrap;
            switch (size)
            {
            case 1:
                wrap = (*buffer == (int8_t)BPIPE_MAGIC);
                break;
            case 2 ... 3:
                wrap = (*buffer == (int16_t)BPIPE_MAGIC);
                break;
            case 4 ... 7:
                wrap = (*buffer == (int32_t)BPIPE_MAGIC);
                break;
            default:
                wrap = (*buffer == (int64_t)BPIPE_MAGIC);
                break;
            }

            if (wrap)
            {
                std::lseek(fd, 0, 0);
                int i = 12;
                std::write(fd, &i, sizeof(int));
                std::lseek(fd, i, 0);
                std::read(fd, buffer, size);
            }
            std::lseek(fd, 0, 0);
            int i;
            std::read(fd, &i, sizeof(int));
            i += size;
            std::write(fd, &i, sizeof(int));
            std::lseek(fd, i, 0);
            return 0;
        }
        else
            return -1;
    }

    int Pipe::write(char *buffer, int size)
    {
        if (!size)
            return 0;
        if (mode == out)
        {
            //等待锁
            //应用锁
            while (lock)
                ;
            lock++;
            //文件锁
            std::lseek(fd, 8, 0);
            int i;
            std::read(fd, &i, sizeof(int));
            while (i)
            {
                std::lseek(fd, 8, 0);
                std::read(fd, &i, sizeof(int));
            }
            std::lseek(fd, 8, 0);
            i = 1;
            std::write(fd, &i, sizeof(int));

            //开始写数据
            std::lseek(fd, 4, 0);
            std::read(fd, &i, sizeof(int));
            std::lseek(fd, i, 0);
            if (i + size >= BPIPE_MAXSIZE)
            {
                uint64_t magic = BPIPE_MAGIC;
                std::write(fd, &magic, sizeof(magic));
                std::lseek(fd, 12, 0);
            }
            std::write(fd, buffer, size);

            //解锁
            std::lseek(fd, 8, 0);
            i= 0;
            std::write(fd, &i, sizeof(int));
            lock--;
            return 0;
        }
        else
            return -1;
    }

    void Pipe::open(std::string path, int mode)
    {
        if (fd)
            std::close(fd);
        this->mode = mode;
        this->fd = std::open(path.c_str(), O_RDWR | O_CREAT);
        this->lock = 0;
        std::lseek(fd, 0, 0);
        if (mode == in)
        {
            std::truncate(name.c_str(), 12);
            int i = 12;
            std::write(fd, &i, sizeof(int));
            std::lseek(fd, i, 0);
        }
        else
        {
            std::lseek(fd, 4, 0);
            int tail;
            std::read(fd, &tail, sizeof(int));
            std::lseek(fd, tail, 0);
        }
    }

    void Pipe::close()
    {
        std::close(fd);
        fd = 0;
    }

    Pipe *Pipe::operator<<(void *buffer)
    {
        this->write((char *)buffer, std::strlen((char *)buffer));
        return this;
    }

};
