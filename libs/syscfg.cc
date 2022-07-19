#include "../includes/syscfg.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

#include <fstream>

#include "../dbg/logs.hh"

namespace bwl
{
    const std::string cfgpath = "/etc/bwl/";

    namespace server
    {

        struct __syscfg
        {
            _Float64x fps;
            const std::string i_fps = "fps";
            uint64_t size[2];
            const std::string i_monisize = "monisize";
        } syscfg;

        void loadCfgFromFs(uint64_t opt)
        {
            syscfg.fps = 60;
            //检查配置文件
            if (stat(cfgpath.c_str(), nullptr))
                mkdir(cfgpath.c_str(), 0755);
            std::fstream cfgfile(cfgpath + "server.cfg", std::ios::in | std::ios::out);
            if (!cfgfile.good()) //文件不存在就新建配置
            {
                creat((cfgpath + "server.cfg").c_str(), 0755);
                cfgfile.close();
                cfgfile.open(cfgpath + "server.cfg", std::ios::in | std::ios::out);
                cfgfile << "fps " << syscfg.fps << '\n'
                        << "monisize " << syscfg.size[0] << ' ' << syscfg.size[1] << '\n';
            }
            while (!cfgfile.eof())
            {
                std::string str;
                cfgfile >> str;
                if ((str == syscfg.i_fps) && (opt & FPS_CFG))
                {
                    cfgfile >> syscfg.fps;
                    continue;
                }
                if ((str == syscfg.i_monisize) && (opt & MONISIZE_CFG))
                {
                    cfgfile >> syscfg.size[0];
                    cfgfile >> syscfg.size[1];
                }
            }
        }

        _Float64x getFPS()
        {
            return syscfg.fps;
        }

        void getMoniSize(uint64_t *width, uint64_t *height)
        {
            *width = syscfg.size[0];
            *height = syscfg.size[1];
        }

        void setMoniSize(uint64_t width, uint64_t height)
        {
            syscfg.size[0] = width;
            syscfg.size[1] = height;
        }
    };
};
