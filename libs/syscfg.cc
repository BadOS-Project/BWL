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
        } syscfg;

        void loadCfgFromFs(uint64_t opt)
        {
            syscfg.fps = 60;
            //检查配置文件
            if (stat(cfgpath.c_str(), nullptr))
                mkdir(cfgpath.c_str(), 0755);
            std::fstream cfgfile(cfgpath + "server.cfg", std::ios::in | std::ios::out);
            if(!cfgfile.good())//文件不存在就新建配置
            {
                creat((cfgpath + "server.cfg").c_str(), 0755);
                cfgfile.close();
                cfgfile.open(cfgpath + "server.cfg", std::ios::in | std::ios::out);
                cfgfile << "fps 60\n";
            }
            bwl::log("here");
            while (!cfgfile.eof())
            {
                std::string str;
                cfgfile >> str;
                if ((str == syscfg.i_fps) && (opt & FPS_CFG))
                {
                    cfgfile >> syscfg.fps;
                    continue;
                }
            }
        }

        _Float64x getFPS()
        {
            return syscfg.fps;
        }
    };
};
