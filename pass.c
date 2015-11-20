#include "header.h"

int check_sum (struct tcp_head tcp_h, int data_len)
{
    unsigned int i, sum=0, cksum;
    unsigned short int cksum_arr[76];

    memcpy(cksum_arr, &tcp_h, 152);
    if (data_len == 0)
    {
        for (i=0; i<12; i++)
            sum += cksum_arr[i];
    }
    else
    {
        for (i=0; i<76; i++)
            sum += cksum_arr[i];
    }
    
    cksum = sum>>16;
    sum = sum & 0x0000FFFF;

    cksum = sum>>16;
    sum = sum & 0x0000FFFF;

    cksum = cksum + sum;
    return cksum;
}