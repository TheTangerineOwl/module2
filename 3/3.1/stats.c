#include "stats.h"

__mode_t getMode(const char* filename)
{
    struct stat buffer;
    
    if (stat(filename, &buffer))
    {
        puts("Error reading file mode!");
        return 0;
    }

    return buffer.st_mode;
}

void printBinary(const __mode_t mode) {
    //int bits = sizeof(__mode_t) * 8; 

    for (int i = 11; i >= 0; i--) {
        printf("%d", (mode >> i) & 1); 
    }
}

void printFileMode(const char* filename)
{
    __mode_t mode;
    if ((mode = getMode(filename)) == 0)
        return;
    printf("Файл: %s\n", filename);
    printMode(mode);
}

int convertToMode(const char* strMode, __mode_t* result)
{
    __mode_t res = 0;
    if (strlen(strMode) < 9)
    {
        printf("Incorrect permissions!\n");
        return 0;
    }
    if (strMode[0] == 'r') res |= S_IRUSR;
    if (strMode[1] == 'w') res |= S_IWUSR;
    if (strMode[2] == 'x') res |= S_IXUSR;
    else if (strMode[2] == 's') res |= S_ISUID;
    if (strMode[3] == 'r') res |= S_IRGRP;
    if (strMode[4] == 'w') res |= S_IWGRP;
    if (strMode[5] == 'x') res |= S_IXGRP;
    else if (strMode[5] == 's') res |= S_ISGID;
    if (strMode[6] == 'r') res |= S_IROTH;
    if (strMode[7] == 'w') res |= S_IWOTH;
    if (strMode[8] == 'x') res |= S_IXOTH;
    else if (strMode[8] == 's') res |= __S_ISVTX;
    *result = res;
    return 1;
}

void printMode(const __mode_t mode)
{
    printf("Битовое представление: ");
    printBinary(mode);
    printf("\nЦифровое представление: %04o\n", mode & 07777);
    printf("Буквенное представление: ");

    printf(mode & S_IRUSR ? "r" : "-");
    printf(mode & S_IWUSR ? "w" : "-");
    if (mode & S_ISUID)
        printf("s");
    else
        printf(mode & S_IXUSR ? "x" : "-");
    printf(mode & S_IRGRP ? "r" : "-");
    printf(mode & S_IWGRP ? "w" : "-");
    if (mode & S_ISGID)
        printf("s");
    else
        printf(mode & S_IXGRP ? "x" : "-");
    printf(mode & S_IROTH ? "r" : "-");
    printf(mode & S_IWOTH ? "w" : "-");
    if (mode & __S_ISVTX)
        printf("t");
    else
        printf(mode & S_IXOTH ? "x" : "-");
    puts("");
}

int parseOption(char* option, __mode_t* mode)
{
    char *c = option;
    __mode_t ugoMask = 0;
    __mode_t modeMask = 0;
    int opCode = 0;
    int flagUGO = 0, flagOP = 0, flagPERM = 0;
    if (!mode)
    {
        puts("Can't get initial permission mode!\n");
        return 0;
    }
    while (*c)
    {
        switch (*c)
        {
        case 'u':
            ugoMask |= S_ISUID | S_IRWXU;  // 04700;
            flagUGO = 1;
            break;
        case 'g':
            ugoMask |= S_ISGID | S_IRWXG;  // 02070;
            flagUGO = 1;
            break;
        case 'o':
            ugoMask |= __S_ISVTX | S_IRWXO;  // 01007;
            flagUGO = 1;
            break;
        case 'a':
            ugoMask |= S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID | S_ISGID | __S_ISVTX;  // 07777;
            flagUGO = 1;
            break;
        case 'r':
            modeMask |= S_IRUSR | S_IRGRP | S_IROTH;  // 00444;
            flagPERM = 1;
            break;
        case 'w':
            modeMask |= S_IWUSR | S_IWGRP | S_IWOTH; // 00222;
            flagPERM = 1;
            break;
        case 'x':
            modeMask |= S_IXUSR | S_IXGRP | S_IXOTH;  // 00111;
            flagPERM = 1;
            break;
        case 's':
            modeMask |= S_ISUID | S_ISGID;  // 06000;
            flagPERM = 1;
            break;
        case 't':
            modeMask |= __S_ISVTX;  // 01000;
            flagPERM = 1;
            break;
        case '+':
        case '-':
        case '=':
            if (flagOP)
            {
                printf("More than one operation in %s!\n", option);
                return 0;
            }
            if (flagUGO && !flagPERM)
            {
                flagOP = 1;
                if (*c == '+')
                    opCode = 1;
                else if (*c == '-')
                    opCode = 2;
                else
                    opCode = 3;
                break;
            }
            else
            {
                printf("Incorrect format of option %s! %d%d%d\n", option, flagUGO, flagOP, flagPERM);
                return 0;
            }
        default:
            printf("Incorrect symbol %c in option %s!\n", *c, option);
            return 0;
        }
        *c++;
    }
    __mode_t mask = ugoMask & modeMask;
    if (opCode == 1)
    {
        *mode = *mode | mask;
        return opCode;
    }
    if (opCode == 2)
    {
        *mode = *mode & ~mask;
        return opCode;
    }
    if (opCode == 3)
    {
        *mode = mask | (*mode & ~ugoMask);
        return opCode;
    }
    return 0;
}

int parseChmod(const int argc, char* argv[])
{
    __mode_t mode;
    if (argc == 4 && sscanf(argv[3], "%o", &mode))
    {
        puts("New mode: ");
        printMode(mode);
        return 1;
    }
    else
    {
        mode = getMode(argv[argc - 1]);
        for (size_t i = 2; i < argc - 1; i++)
        {
            if (!parseOption(argv[i], &mode))
                return 0;
        }
        puts("New mode: ");
        printMode(mode);
    }
    return 1;
}