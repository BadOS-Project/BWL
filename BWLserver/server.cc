#include "server.hh"

int main(int argc, char **argv)
{
    argc--, argv++;
    for (int i = 0; i < argc; i++)
    {
        if(std::string(argv[i]) == "-c" || std::string(argv[i]) == "--card")
        {
            i++;
            monitor_device += argv[i];
        }
    }
    switch_to_daemon();
    bwl::log("Bwl starting.");
    bwl::log("making bwl device.");
    make_bwl_dev();
    start_bwl_server();
    sleep(1);
    bwl::bwl_exit(0);
}
