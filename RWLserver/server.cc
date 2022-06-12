#include "server.hh"

int main(int argc, char **argv)
{
    switch_to_daemon();
    start_rwl_server();
}
