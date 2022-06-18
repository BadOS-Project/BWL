#include "server.hh"

int main(int argc, char **argv)
{
    switch_to_daemon();
    start_bwl_server();
}
