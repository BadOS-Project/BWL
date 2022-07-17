#include "server.hh"

#define USEAGE                                                                                \
    "usage: bwl-server [options]" << std::endl                                                \
                                  << "The options contains:" << std::endl                     \
                                  << "\t-h | --help\tTo print this document." << std::endl    \
                                  << "\t-v | --version\tTo print version data." << std::endl \
                                  << "\t-c | --card\tTo choose which video card to use(card 0 as default)." << std::endl

int main(int argc, char **argv)
{
    argc--, argv++;
    monitor_device = "0";
    for (int i = 0; i < argc; i++)
    {
        if (std::string(argv[i]) == "-c" || std::string(argv[i]) == "--card")
        {
            i++;
            monitor_device = argv[i];
        }

        if (std::string(argv[i]) == "-v" || std::string(argv[i]) == "--version")
        {
            std::cout << "Bad WayLand " << BWL_VERSION << std::endl
                      << "Bad Wayland Server " << BWLS_VERSION << std::endl;
            exit(0);
        }

        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
        {
            std::cout << USEAGE;
            exit(0);
        }
    }
    switch_to_daemon();
    bwl::log("Bwl starting.");
    bwl::log("making bwl device.");
    make_bwl_dev();
    bwl::log("starting bad wayland server.");
    start_bwl_server();
    bwl::log("OK");
    sleep(2);
    bwl::destoryDisplay();
    bwl::bwl_exit(0);
}
