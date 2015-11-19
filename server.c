#include "header.h"

int main (void)
{
	int sock_des, sock_client;
	int byte_rec = 1;
	struct sockaddr_in server, client;
	struct tcp_head tcp_h;
	socklen_t addr_size;
	/* create socket to client */
	sock_des = socket (AF_INET, SOCK_STREAM,0);
	if (sock_des == -1)
	{
		printf ("Failed to create socket.\n");
		return 1;
	}

	/* set fields in sockaddr_in struct */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons (PORT_NO);

	/* bind socket */
	bind (sock_des, (struct sockaddr *)&server, sizeof server);
	if (sock_des == -1)
	{
		printf ("Bind failed.\n");
		close (sock_des);
		return 1;
	}
	/* listen on sock_des */
	if (listen (sock_des, 3) == -1)
	{
		printf ("Listen failed.\n");
		close (sock_des);
		return 1;
	}
	/* accept incoming connection */
	addr_size = sizeof client;
	sock_client = accept (sock_des, (struct sockaddr *)&client, &addr_size);

	while (byte_rec > 0)
	{
		byte_rec = recv (sock_client, &tcp_h, sizeof tcp_h, 0);
		printf ("%x %x %x %x \n%x %x %x %x %x\n", tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
			tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);
	}


	/* clenup */
	close (sock_des);
	return 0;
}