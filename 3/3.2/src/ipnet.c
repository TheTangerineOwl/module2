#include "ipnet.h"

#define GET_BETWEEN(MIN, MAX) rand() % (MIN - MAX + 1) + MIN

#define GET_OCTET(var, MIN, MAX) \
    var << 8 | GET_BETWEEN(MIN, MAX)

void swap(u_int8_t* minA, u_int8_t* maxB)
{
    u_int8_t* buffer;
    if (minA > maxB)
    {
        buffer = minA;
        minA = maxB;
        maxB = buffer;
    }
}

IPAddr_t generateIPv4(
    u_int8_t minOctet1,
    u_int8_t maxOctet1,
    u_int8_t minOctet2,
    u_int8_t maxOctet2,
    u_int8_t minOctet3,
    u_int8_t maxOctet3,
    u_int8_t minOctet4,
    u_int8_t maxOctet4
)
{
    u_int8_t buffer;
    IPAddr_t res = 0;
    swap(&minOctet1, &maxOctet1);
    swap(&minOctet2, &maxOctet2);
    swap(&minOctet3, &maxOctet3);
    swap(&minOctet4, &maxOctet4);
    res = GET_OCTET(res, minOctet1, maxOctet1);
    res = GET_OCTET(res, minOctet2, maxOctet2);
    res = GET_OCTET(res, minOctet3, maxOctet3);
    res = GET_OCTET(res, minOctet4, maxOctet4);
    return res;
}

void printAddr(const IPAddr_t address)
{
    u_int8_t first = address >> 24 & 0xFF;
    u_int8_t second = address >> 16 & 0xFF;
    u_int8_t third = address >> 8 & 0xFF;
    u_int8_t fourth = address & 0xFF;
    printf("%d.%d.%d.%d", first, second, third, fourth);
}

int parseAddress(char* strAddr, IPAddr_t* result)
{
    IPAddr_t res = 0;
    char* str;
    u_int8_t octet = 0;
    str = strtok(strAddr, ".");
    do
    {
        if (!sscanf(str, "%hhu", &octet))
            return -1;
        res = res << 8 | octet;
    } while ((str = strtok(NULL, ".")) != NULL);
    *result = res;
    return 0;
}