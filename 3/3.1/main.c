#include "stats.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 0;
    char* flag = argv[1];
    if (argc == 2)
        printFileMode(flag);
    else if (strncmp(flag, "--file", 7) == 0)
    {
        char* filename = argv[2];
        printFileMode(filename);
    }
    else if (strncmp(flag, "--print-mode", 13) == 0)
    {
        char* strMode = argv[2];
        __mode_t mode;
        if (!sscanf(strMode, "%o", &mode) && !convertToMode(strMode, &mode))
        {
            printf("Couldn't convert mode!\n");
            return 1;
        }
        printMode(mode);
    }
    else if (strncmp(flag, "--chmod", 8) == 0)
    {
        if (argc < 4)
        {
            puts("Not enough arguments!");
            return 1;
        }
        char* filename = argv[argc - 1];
        printFileMode(filename);
        if (!parseChmod(argc, argv))
        {
            puts("Couldn't parse chmod options!");
            return 1;
        }
    }
}