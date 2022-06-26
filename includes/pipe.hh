#ifndef __pipe_hh_
#define __pipe_hh_

#include <iostream>

/**
 * @brief 管道
 *
 * 这个管道是自己实现的
 *
 */

namespace bwl
{
    class Pipe
    {
    private:
        int fd;
        int mode;
        int lock;
    public:
        const int in = 055;
        const int out = 077;

        Pipe(std::string, int mode);
        ~Pipe();

        int read(char *buffer, int size);
        int write(char *buffer, int size);

        void open(std::string path, int mode);
        void close();
    };
};

#endif
