#ifndef __pipe_hh_
#define __pipe_hh_

#include <iostream>

/**
 * @brief 管道
 *
 * 这个管道是自己实现的
 * 
 * 管道线程安全，但是进程不安全
 * #define BPIPE_MAGIC 0x8cffa60091dd00c4
 * #define BPIPE_MAXSIZE 0x1000000
 * MAGIC常量用于在管道文件中标记管道数据绕回，
 * MAXSIZE表示管道文件的最大大小。
 * 由此可见，并没有机制来判断绕回的管道数据是否触及
 * 还未绕回的接收端的文件指针位置；
 * 不过这最大文件大小这么大(256MB)应该不会出现这样的情况吧。
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
        std::string name;
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
