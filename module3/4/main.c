
#include "interpreter.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    command_s commands[MAX_PIPES];
    char input[MAX_INPUT];
    // char* argv[MAX_ARGS];

    // int fd_fifo;
    // char buf[MAX_INPUT];

    // unlink("/tmp/fifo0001.1");

    printf("Для выхода из интерпретатора ввести: exit \n");

    while (1)
    {
        printf("Введите команду: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        if (strlen(input) == 0)
        {
            continue;
        }
        int argc = parse_input(input, commands);
        if (argc == 0)
        {
            continue;
        }

        execute_pipline(commands, argc);
    }

    printf("Конец работы интерпритатора.\n");
    return 0;
}