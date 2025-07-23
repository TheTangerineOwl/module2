#ifndef STATS_H
#define STATS_H

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

__mode_t getMode(const char* filename);
void printBinary(const __mode_t mode);
void printMode(const __mode_t mode);
void printFileMode(const char* filename);
int convertToMode(const char* strMode, __mode_t* result);
int parseOption(char* option, __mode_t* mode);
int parseChmod(const int argc, char* argv[]);

#endif