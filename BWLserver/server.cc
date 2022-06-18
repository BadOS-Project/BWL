#include "server.hh"

int main(int argc, char **argv)
{
    switch_to_daemon();
    bwl::log("Bwl starting.");
    bwl::log("making bwl device.");
    make_bwl_dev();
    bwl::bwl_exit(0);
}
