#ifndef __header__
#define __header__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_NO 56565
#define SERV_IP "129.120.151.94"

struct tcp_head {
    short int source_port, dest_port;
    int seq_num, ack_num;
    short int flags, window;
    short int chksum, urg_ptr;
    int options;
};

#endif