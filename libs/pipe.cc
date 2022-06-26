#include "../includes/pipe.hh"

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
#include <cstdio>
#include <unistd.h>

namespace bwl
{
    Pipe::Pipe(std::string path, int mode)
    {
        this->mode = mode;
        this->fd = open(path.c_str(), (mode == out) ? O_WRONLY : O_RDONLY);
        this->lock = 0;
    }

    Pipe::~Pipe()
    {
        close(fd);
    }

};
