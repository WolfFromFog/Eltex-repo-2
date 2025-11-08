#include "ips.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char*  adress;
    char*  mask;
    int N;
    network_config net;
    srand((unsigned int)time(NULL));
    if (argc > 1)
    {
        if(argc == 4)
        {
        adress  = argv[1];
        mask    = argv[2];
        N  = atoi(argv[3]);
        
        net_tester(&net, adress, mask, N);

       // printf("%s\n%s\n%d\n", adress, mask, N);
        }
        else
        {
            printf("Ошибка: неверное количество аргументов!\n");
            return -1;
        }
        

        
    }
    else
    {   
        adress = malloc(sizeof(char) * 16);
        mask = malloc(sizeof(char) * 16);
        char temp[16];
        printf("Введите адрес сети: ");
        scanf("%16s",&temp);
        strcpy(adress, temp);
        printf("Введите маску сети: ");
        scanf("%16s",&temp);
        strcpy(mask, temp);
        printf("Введите количество пакетов: ");
        scanf("%d",&N);

        net_tester(&net, adress, mask, N);

        //printf("%s\n%s\n%d\n", adress, mask, N);

        free(adress);
        free(mask);
    }

    return 0;
}