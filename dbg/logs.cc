#include "logs.hh"

#define RWLSV_GR "[rwl-server] "
#define RWLSV_RD "[rwl-server] "

namespace rwl
{
    void rwl_exit(int code)
    {
        std::cout << RWLSV_GR << "exit with code " << code << std::endl;
        exit(code);
    }

    void log(std::string log)
    {
        std::cout << RWLSV_GR << log << std::endl;
    }

    void err(std::string err)
    {
        std::cerr << RWLSV_RD << err << std::endl;
    }
};
