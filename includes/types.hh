#ifndef __types_hh_
#define __types_hh_

#include <iostream>
#include <vector>

extern std::string monitor_device;

#define SIGBWLREQ 38
#define SIGREQRECV 35
#define REQMAGIC 0xa68c5cce7be74433

#define BWLDIR (std::string("/dev/bwl(") + monitor_device + ")")
#define PAGEDIR (std::string("/dev/bwl(") + monitor_device + ")/pages/")
#define FRMDIR (std::string("/dev/bwl(") + monitor_device + ")/frames/")

namespace bwl
{
    typedef unsigned long long id_t;
};

#endif