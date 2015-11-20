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
#define CLIENT_IP "129.120.151.95"
#define DATA_LEN 128
#define BUFFER_LEN 2048

struct tcp_head {
	unsigned short int source_port, dest_port;
	unsigned int seq_num, ack_num;
	unsigned short int flags, window;
	unsigned short int chksum, urg_ptr;
	unsigned int options;
	char data[128];
};

int check_sum (struct tcp_head tcp_h, int data_len);
#endif