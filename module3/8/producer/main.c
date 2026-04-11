#include <stdlib.h>
#include <stdio.h>
#include "producer.h"
// #include <stream.h>
#include <string.h>

int main(int argc, char *argv[])
{
    srand(10);
    char *filename;
    int numOfProducts;
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
    int c_wait = 1;
    while (c_wait)
    {
        /* code */
    }

    free(filename);
    return 0;
}