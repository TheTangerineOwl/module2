#ifndef STATS_H
#define STATS_H

#include <sys/stat.h>
#include <stdio.h>

__mode_t getMode(const char* filename);
void printBinary(const __mode_t mode);
void printMode(const char* filename);

#endif