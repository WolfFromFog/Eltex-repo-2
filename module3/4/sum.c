#include <stdio.h>
#include <string.h>

int main(int args, char *argv[])
{
    double result = 0;
    char extra;
    if (args > 0)
    {
        for (int i = 1; i < args; i++)
        {
            double a;
            if (sscanf(argv[i], "%lf%c", &a, &extra) == 1)
            {
                result += a;
            }
        }
    }
    printf("Результат суммы: %lf\n", result);
    return result;
}