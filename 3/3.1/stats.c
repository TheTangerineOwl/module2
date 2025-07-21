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

void printMode(const char* filename)
{
    __mode_t mode;
    if ((mode = getMode(filename)) == 0)
        return;
    printf("Файл: %s\n", filename);
    printf("Битовое представление: ");
    printBinary(mode);
    printf("\nЦифровое представление: %04o\n", mode & 0777);
    printf("Буквенное представление: ");

    printf(mode & S_IRUSR ? "r" : "-");
    printf(mode & S_IWUSR ? "w" : "-");
    printf(mode & S_IXUSR ? "x" : "-");
    printf(mode & S_IRGRP ? "r" : "-");
    printf(mode & S_IWGRP ? "w" : "-");
    printf(mode & S_IXGRP ? "x" : "-");
    printf(mode & S_IROTH ? "r" : "-");
    printf(mode & S_IWOTH ? "w" : "-");
    printf(mode & S_IXOTH ? "x" : "-");
    puts("");
}