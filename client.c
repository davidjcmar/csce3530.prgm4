#include "header.h"

int main (void)
{
	int sock_des;
	int send_byte, recv_byte, remain_byte, n;
	struct sockaddr_in server;
	struct tcp_head tcp_h;

	/* create socket */
	sock_des = socket (AF_INET,SOCK_STREAM,0);
	if (sock_des == -1)
	{
		printf ("Failed to create socket.\n");

		return 1;
	}
	/* set fields in sockaddr_in */
	server.sin_addr.s_addr = inet_addr(SERV_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT_NO);

	if (connect (sock_des, (struct sockaddr *)&server, sizeof server) == -1)
	{
		printf ("Connection error.\n");
		close (sock_des);
		return 1;
	}
	/* set header fields */
	tcp_h.source_port = PORT_NO;
	tcp_h.dest_port = PORT_NO;
	tcp_h.seq_num = 0;
	tcp_h.ack_num = 0;
	tcp_h.flags = 0b000010u; // set syn bit
	tcp_h.window = 0;
	tcp_h.chksum = 0;
	tcp_h.urg_ptr = 0;

	/* testing *//*
	printf ("%04x", tcp_h.flags);
	/* end testing */

	/* init TCP handshake */
	send_byte = 0;
	recv_byte = 0;
	remain_byte = sizeof tcp_h;
	while (send_byte < remain_byte)
	{
		n = send(sock_des, &tcp_h + send_byte, remain_byte, 0);
		if (n==-1)
			break;
		send_byte += n;
		remain_byte -= n;
	}
	/* receive ACK */
	recv_byte = recv (sock_des, &tcp_h, sizeof tcp_h, 0);
	printf ("%x %x %x %x \n%x %x %x %x %x\n", tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
	tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);
	/* receive */

	/* cleanup */
	close (sock_des);
	return 0;
}