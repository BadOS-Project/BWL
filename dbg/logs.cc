#include "logs.hh"

#define BWLSV_GR "[bwl-server] "
#define BWLSV_RD "[bwl-server] "

extern std::string monitor_device;

namespace bwl
{
    void bwl_exit(int code)
    {
        std::cout << BWLSV_GR << "exit with code " << code << std::endl;
        system((std::string("rm -r '/dev/bwl(") + monitor_device + ")'").c_str());
        exit(code);
    }

    void log(std::string log)
    {
        std::cout << BWLSV_GR << log << std::endl;
    }

    void err(std::string err)
    {
        std::cerr << BWLSV_RD << err << std::endl;
    }
};
