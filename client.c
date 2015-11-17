#include "header.h"

int main (void)
{
    int sock_des;
    int bytes_recv;
    struct sockaddr_in server;
    char test[30];
    memset (test, '\0', 30);
    strcpy (test, "Testing, testing. Hello World");

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
        return 1;
    }
    bytes_recv = send (sock_des, test, strlen(test), 0);
    printf ("%d", bytes_recv);
}