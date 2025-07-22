#include "ipnet.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));
    IPAddr_t address;
    if (argc < 2)
        return 0;
    char* addr = argv[1];
    if (parseAddress(addr, &address) < 0)
    {
        printf("Error parsing the gateway address!");
        return 1;
    }
    else
    {
        printf("Gateway: ");
        printAddr(address);
        puts("");
    }
    if (argc > 2)
    {
        char* subnet = argv[2];
        IPAddr_t mask;
        if (parseAddress(subnet, &mask) < 0)
        {
            puts("Error parsing the mask!");
            return 1;
        }
        size_t count = 10;
        size_t inSubnet = 0;
        if (argc == 4)
            sscanf(argv[3], "%ld", &count);
        IPAddr_t buffer;
        for (size_t i = 0; i < count; i++)
        {
            buffer = generateIPv4(0, 255, 0, 255, 0, 255, 0, 255);
            if ((mask & address) == (mask & buffer))
            {
                inSubnet++;
                printf("Address ");
                printAddr(buffer);
                printf(" is in ");
                printAddr(mask & address);
                printf(" subnet.\n");
            }
            else
            {
                printf("Address ");
                printAddr(buffer);
                printf(" is not in ");
                printAddr(mask & address);
                printf(" subnet.\n");
            }
        }
        printf("There are %ld/%ld (%.2lf%%) in subnet.\n",
            inSubnet, count, (double)inSubnet / count * 100);
    }
    return 0;
}