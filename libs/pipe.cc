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
    Pipe::Pipe(std::string path, int mode)
    {
        open(path, mode);
    }

    Pipe::~Pipe()
    {
        std::close(fd);
    }

    int Pipe::read(char *buffer, int size)
    {
        if (mode == in)
        {
            std::read(fd, buffer, size);
            return 0;
        }
        else
            return -1;
    }

    int Pipe::write(char *buffer, int size)
    {
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
        if(fd)
            std::close(fd);
        this->mode = mode;
        this->fd = std::open(path.c_str(), (mode == out) ? O_WRONLY : O_RDONLY);
        this->lock = 0;
    }

    void Pipe::close()
    {
        std::close(fd);
        fd = 0;
    }

};
