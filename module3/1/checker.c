#include "checker.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void checkNumberType(const char *str)
{
    int int_val;
    double double_val;
    char extra;


    if (sscanf(str, "%d%c", &int_val, &extra) == 1)
    {
        printf("[PID %d] %s - целое, %d x 2 = %d \n", getpid(), str, int_val, int_val*2);
        return;
    }

    if (sscanf(str, "%lf%c", &double_val, &extra) == 1)
    {
        printf("[PID %d] %s - вещественное, %.4f x 2 = %.4f \n", getpid(), str, double_val, double_val * 2);
        return;
    }

    printf("[PID %d] %s  - строка \n", getpid(), str);
}
