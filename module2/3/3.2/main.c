#include "ips.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    char*  adress    = (char*)malloc(sizeof(char));
    char*  mask     = (char*)malloc(sizeof(char));
    uint16_t N;

    if (argc > 1)
    {

        adress  = argv[1];
        mask    = argv[2];
        N  = (uint16_t)argv[3];

        printf("%s\n%s\n%d", adress, mask, N);
        
    }
    else
    {

    }
    
    free(adress);
    free(mask);

    return 0;
}