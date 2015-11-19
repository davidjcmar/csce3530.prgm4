#include "header.h"

int main (void)
{
	int sock_des, sock_client;
	int send_byte, recv_byte, remain_byte, n;
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
	
	/* init TCP connection */ 
	recv_byte = recv (sock_client, &tcp_h, sizeof tcp_h, 0);
	printf ("%x %x %x %x \n%x %x %x %x %x\n", tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
	tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);
	/* set header fields */
	tcp_h.source_port = PORT_NO;
	tcp_h.dest_port = PORT_NO;
	tcp_h.seq_num = 0;
	tcp_h.ack_num = 0 + 1;
	tcp_h.flags = 0b001010u; // set syn and ack bits
	tcp_h.window = 0;
	tcp_h.chksum = 0;
	tcp_h.urg_ptr = 0;
	tcp_h.options = 0;
	/* return ACK */
	send_byte = 0;
	remain_byte = sizeof tcp_h;
	printf ("Remaining bytes: %d", remain_byte); // testing
	while (send_byte < remain_byte)
	{
		n = send(sock_client, &tcp_h + send_byte, remain_byte, 0);
		if (n==-1)
			break;
		send_byte += n;
		remain_byte -= n;
		printf ("Bytes sent: %d\n", n); // testing
	}
	/* receive ack */
	recv_byte = recv (sock_client, &tcp_h, sizeof tcp_h, 0);
	printf ("%x %x %x %x \n%x %x %x %x %x\n", tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
	tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);	
	/* cleanup */
	close (sock_des);
	close (sock_client);
	return 0;
}