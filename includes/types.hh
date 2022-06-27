#ifndef __types_hh_
#define __types_hh_

#include <iostream>
#include <vector>

extern std::string monitor_device;

#define BWLDIR (std::string("/dev/bwl(") + monitor_device + ")")
#define PAGEDIR (std::string("/dev/bwl(") + monitor_device + ")/pages/")
#define FRMDIR (std::string("/dev/bwl(") + monitor_device + ")/frames/")

namespace bwl
{
    typedef unsigned long long id_t;
};

#endif