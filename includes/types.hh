#ifndef __types_hh_
#define __types_hh_

#include <iostream>
#include <vector>

#define BWL_VERSION "0.0.01"
#define BWLS_VERSION "0.0.01"

extern std::string monitor_device;

#define SIGBWLREQ 38
#define SIGREQRECV 35
#define REQMAGIC 0xa68c5cce7be74433

#define BWLDIR (std::string("/dev/bwl(") + monitor_device + ")")
#define PAGEDIR (std::string("/dev/bwl(") + monitor_device + ")/pages/")
#define FRMDIR (std::string("/dev/bwl(") + monitor_device + ")/frames/")

#define SHMBWLDIR (std::string("/dev_bwl(") + monitor_device + ")")
#define SHMPAGEDIR (std::string("/dev_bwl(") + monitor_device + ")_pages_")
#define SHMFRMDIR (std::string("/dev_bwl(") + monitor_device + ")_frames_")

namespace bwl
{
    typedef unsigned long long id_t;
};

#endif