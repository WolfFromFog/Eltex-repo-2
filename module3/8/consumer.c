#include "factory.h"
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    srand(10);
    char *filename;
    if (argc == 2)
    {
        filename = (char *)malloc(strlen(argv[1]));
        strcpy(filename, argv[1]);
    }
    if (argc == 1)
    {
        char tmp_filename[] = "Storage";
        filename = (char *)malloc(strlen(tmp_filename));
        strcpy(filename, tmp_filename);
    }
    if (argc > 2)
    {
        printf("Ошибка. Неверное количество аргументов. Завершение работы.\n");
        free(filename);
        return 0;
    }
    signal(SIGINT, listener_SIGINT);
    while (c_wait)
    {
    }
    free(filename);
    printf("Работа завершена.\n");
    return 0;
}