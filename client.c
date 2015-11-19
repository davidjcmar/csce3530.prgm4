#include "header.h"

int main (void)
{
	int sock_des;
	int bytes_recv;
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
	tcp_h.flags = 0b000010u;
	tcp_h.window = 0;
	tcp_h.urg_ptr = 0;

	/* testing */
	printf ("%4x", tcp_h.flags);
	/* send */

	/* receive */

	/* cleanup */
	close (sock_des);
	return 0;
}