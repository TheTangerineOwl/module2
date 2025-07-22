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