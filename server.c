#include "header.h"

int main (void)
{
	int sock_des, sock_client;

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

	/* testing */
	format_tcp_h(&tcp_h);
	/* clenup */
	close (sock_des);
	return 0;
}