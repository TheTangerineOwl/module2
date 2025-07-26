#ifndef CALC_H
#define CALC_H

#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>

#define EPSILON 1e-8
#define MAX_ARGS 5

typedef struct
{
    const char *str;
    double* (*op) (int, ...);
} Operations;

extern Operations operations[];

double* sum(int argc, ...);
double* sub(int argc, ...);
double* mult(int argc, ...);
double* divide(int argc, ...);

#endif