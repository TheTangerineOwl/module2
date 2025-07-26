#include "calc.h"

double *sum(int argc, ...)
{
    double *res = (double*)malloc(sizeof(double));
    *res = 0.0;
    va_list args;
    va_start(args, argc);
    for (int i = 0; i < argc; i++)
        *res += va_arg(args, double);
    va_end(args);
    return res;
}

double *sub(int argc, ...)
{
    va_list args;
    va_start(args, argc);
    double *res = (double*)malloc(sizeof(double));
    *res = va_arg(args, double);
    for (int i = 1; i < argc; i++)
        *res -= va_arg(args, double);
    va_end(args);
    return res;
}

double *mult(int argc, ...)
{
    double *res = (double*)malloc(sizeof(double));
    *res = 1.0;
    va_list args;
    va_start(args, argc);
    for (int i = 0; i < argc; i++)
        *res *= va_arg(args, double);
    va_end(args);
    return res;
}

double *divide(int argc, ...)
{
    va_list args;
    va_start(args, argc);
    double *res = (double*)malloc(sizeof(double));
    *res = va_arg(args, double);
    double d;
    for (int i = 1; i < argc; i++)
    {
        d = va_arg(args, double);
        if ((d > 0.0 && d <= EPSILON) || (d < 0.0 && d >= -EPSILON))
        {
            free(res);
            return NULL;
        }
        *res /= d;
    }
    va_end(args);

    return res;
}