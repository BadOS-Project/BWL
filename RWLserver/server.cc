#include "server.hh"

int main(int argc, char **argv)
{
    switch_to_daemon();
    fbinfo.bufferfile = open("/dev/fb0", O_RDWR);
    if(!fbinfo.bufferfile)
    {
        rwl::err("cannot open device /dev/fb0 .");
        rwl::rwl_exit(-2);
    }
    rwl::log("succeeded open device /dev/fb0 .");

    fb_var_screeninfo vinfo;
    fb_fix_screeninfo finfo;
    ioctl(fbinfo.bufferfile, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fbinfo.bufferfile, FBIOGET_VSCREENINFO, &vinfo);

    mapbuffer(&vinfo, &finfo);
}
