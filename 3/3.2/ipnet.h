#ifndef IPNET_H
#define IPNET_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

typedef u_int32_t IPAddr_t;

void swap(u_int8_t* minA, u_int8_t* maxB);
IPAddr_t generateIPv4(
    u_int8_t minOctet1,
    u_int8_t maxOctet1,
    u_int8_t minOctet2,
    u_int8_t maxOctet2,
    u_int8_t minOctet3,
    u_int8_t maxOctet3,
    u_int8_t minOctet4,
    u_int8_t maxOctet4
);
void printAddr(const IPAddr_t address);
int parseAddress(char* strAddr, IPAddr_t* result);

#endif