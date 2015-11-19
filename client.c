#include "header.h"

int main (void)
{
	int sock_des;
	int send_byte, recv_byte, remain_byte, n;
	struct sockaddr_in server;
	struct tcp_head tcp_h;
	char buffer[DATA_LEN];
	char payload[BUFFER_LEN];
	int i=0,j=0;

	memset(buffer,'\0',DATA_LEN);
	memset(payload,'\0',BUFFER_LEN);
	scanf("%2048[^\n]", payload);
	//printf ("%s\n", payload); // testing
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
	tcp_h.options = 0;
	tcp_h.chksum = check_sum(tcp_h, 0);

	/* init TCP handshake */
	send_byte = 0;
	remain_byte = sizeof tcp_h - DATA_LEN;
	while (send_byte < remain_byte)
	{
		n = send(sock_des, &tcp_h + send_byte, remain_byte, 0);
		if (n==-1)
			break;
		send_byte += n;
		remain_byte -= n;
	}
	/* receive ACK */
	recv_byte = recv (sock_des, &tcp_h, sizeof tcp_h - DATA_LEN, 0);
//	printf ("%d", recv_byte);
	printf ("Header:\n");
	printf ("%d %d 0x%04x 0x%04x \n0x%02x 0x%02x 0x%02x 0x%02x 0x%04x\n",\
		tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
		tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);
	/* return ACK */
	tcp_h.seq_num += 1;
	tcp_h.ack_num += 1;
	tcp_h.flags = 0b001000u; // set ack bit
	tcp_h.chksum = check_sum(tcp_h, 0);

	send_byte = 0;
	remain_byte = sizeof tcp_h - DATA_LEN;
	while (send_byte < remain_byte)
	{
		n = send(sock_des, &tcp_h + send_byte, remain_byte, 0);
		if (n==-1)
			break;
		send_byte += n;
		remain_byte -= n;
	}

	/* send data */
	memset (tcp_h.data, '\0', DATA_LEN);
	tcp_h.seq_num = -1;
	tcp_h.ack_num = 0;
	tcp_h.flags = 0b000000u;
	while (i < strlen(payload))
	{
		/* package payload */
		for (j=0; j<DATA_LEN, i<strlen(payload); i++, j++)
		{
			tcp_h.data[j] = payload[i];
		}
		/* set header values */
		tcp_h.seq_num += 1;
		tcp_h.ack_num = 0;
		tcp_h.chksum = check_sum(tcp_h, sizeof(tcp_h.data));
		/* send packet */
		send_byte = 0;
		remain_byte = sizeof tcp_h;
		while (send_byte < remain_byte)
		{
			n = send(sock_des, &tcp_h + send_byte, remain_byte, 0);
			if (n==-1)
				break;
			send_byte += n;
			remain_byte -= n;
		}
		/* receive ack */
		recv_byte = recv (sock_des, &tcp_h, sizeof tcp_h - DATA_LEN, 0);
		printf ("Header:\n");
		printf ("%d %d 0x%04x 0x%04x \n0x%02x 0x%02x 0x%02x 0x%02x 0x%04x\n",\
		tcp_h.source_port, tcp_h.dest_port, tcp_h.seq_num,\
		tcp_h.ack_num, tcp_h.flags, tcp_h.window, tcp_h.chksum, tcp_h.urg_ptr, tcp_h.options);
	}

	/* close TCP connection */

	/* cleanup */
	close (sock_des);
	return 0;
}