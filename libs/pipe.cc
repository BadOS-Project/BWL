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
};

#include <cstdio>

namespace bwl
{

#define BPIPE_MAGIC 0x8cffa60091dd00c4
#define BPIPE_MAXSIZE 0x1000000

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
                wrap = (*buffer == (int8_t)BPIPE_MAGIC) ? true : false;
                break;
            case 2 ... 3:
                wrap = (*buffer == (int16_t)BPIPE_MAGIC) ? true : false;
                break;
            case 4 ... 7:
                wrap = (*buffer == (int32_t)BPIPE_MAGIC) ? true : false;
                break;
            default:
                wrap = (*buffer == (int64_t)BPIPE_MAGIC) ? true : false;
                break;
            }
            if (*buffer == 0x8c)
            {
                std::lseek(fd, 0, 0);
                std::read(fd, buffer, size);
            }
            return 0;
        }
        else
            return -1;
    }

    int Pipe::write(char *buffer, int size)
    {
        if (!size)
            return 0;
        struct std::stat buf;
        std::stat(name.c_str(), &buf);
        if(buf.st_size + size >= BPIPE_MAXSIZE)
        {
            uint64_t magic = BPIPE_MAGIC;
            std::write(fd, &magic, sizeof(magic));
            std::lseek(fd, 0, 0);
        }
        if (mode == out)
        {
            while (lock)
                ;
            lock++;
            std::write(fd, buffer, size);
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
        this->fd = std::open(path.c_str(), ((mode == out) ? O_WRONLY : O_RDONLY) | O_CREAT);
        this->lock = 0;
    }

    void Pipe::close()
    {
        std::close(fd);
        fd = 0;
    }

};
