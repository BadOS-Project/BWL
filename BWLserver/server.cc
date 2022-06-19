#include "server.hh"

int main(int argc, char **argv)
{
    switch_to_daemon();
    bwl::log("Bwl starting.");
    bwl::log("making bwl device.");
    make_bwl_dev();
    start_bwl_server();
    sleep(1);
    bwl::bwl_exit(0);
}
