#include "checker.h"
#include <string.h>
#include <ctype.h>

int isNumber(const char *str)
{
    int len = strlen(str);
    int pointCounter = 0;

    for (int i = 0; i < len; i++)
    {
        if (!isdigit(str[i]))
        {
            if (str[i] == '.' && i != 0)
            {
                pointCounter++;
            }
            else
            {
                return 1;
            }
        }
        if (pointCounter > 1)
        {
            return 1;
        }
    }

    return 0;
}
