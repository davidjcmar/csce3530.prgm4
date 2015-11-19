#include "header.h"

int main (void)
{
	int sock_des, sock_client;
	int send_byte, recv_byte, remain_byte, n;
	struct sockaddr_in server, client;
	struct tcp_head tcp_h;
	socklen_t addr_size;
	char buffer[DATA_LEN];
	char payload[BUFFER_LEN];
	/* init buffers */
	memset (buffer, '\0', DATA_LEN);
	memset (payload, '\0', BUFFER_LEN);
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
	recv_byte = recv (sock_client, &tcp_h, sizeof tcp_h - DATA_LEN, 0);
	printf ("Header:\n");
	printf ("%d %d 0x%04x 0x%04x \n0x%02x 0x%02x 0x%02x 0x%02x 0x%04x\n", tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
	tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);
	/* set header fields */
	tcp_h.source_port = PORT_NO;
	tcp_h.dest_port = PORT_NO;
	tcp_h.seq_num = 0;
	tcp_h.ack_num = 0;
	tcp_h.flags = 0b001010u; // set syn and ack bits
	tcp_h.window = 0;
	tcp_h.chksum = 0;
	tcp_h.urg_ptr = 0;
	tcp_h.options = 0;
	tcp_h.chksum = check_sum(tcp_h, 0);
	/* return ACK */
	send_byte = 0;
	remain_byte = sizeof tcp_h;
//	printf ("Remaining bytes: %d", remain_byte); // testing
	while (send_byte < remain_byte)
	{
		n = send(sock_client, &tcp_h + send_byte, remain_byte, 0);
		if (n==-1)
			break;
		send_byte += n;
		remain_byte -= n;
//		printf ("Bytes sent: %d\n", n); // testing
	}
	/* receive ack */
	recv_byte = recv (sock_client, &tcp_h, sizeof tcp_h - DATA_LEN, 0);
	printf ("Header:\n");
	printf ("%d %d 0x%04x 0x%04x \n0x%02x 0x%02x 0x%02x 0x%02x 0x%04x\n",\
		tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
		tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);

	/* receive data */
	while (recv_byte > 0)
	{
		recv_byte = recv (sock_client, &tcp_h, sizeof tcp_h, 0);
		printf ("Header:\n");
		printf ("%d %d 0x%04x 0x%04x \n0x%02x 0x%02x 0x%02x 0x%02x 0x%04x\n", \
			tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
			tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);
		strcpy(buffer, tcp_h.data);
		strcat(payload, buffer);
		printf ("%s\n", buffer);
		memset(buffer, '\0', DATA_LEN);

		/* send ACK */
		tcp_h.seq_num = 0;
		tcp_h.flags = 0b001000u;
		tcp_h.ack_num = strlen(buffer);
		tcp_h.chksum = check_sum(tcp_h, 0);
		
		send_byte = 0;
		remain_byte = sizeof tcp_h;
		while (send_byte < remain_byte)
		{
			n = send(sock_client, &tcp_h + send_byte, remain_byte, 0);
			if (n==-1)
				break;
			send_byte += n;
			remain_byte -= n;
		}
	}

	/* close TCP connection */
	printf ("%s\n", payload);
	/* cleanup */
	close (sock_des);
	close (sock_client);
	return 0;
}