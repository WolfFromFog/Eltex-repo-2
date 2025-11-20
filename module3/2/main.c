
#include "interpreter.h"
#include <stdio.h>
#include <string.h>

int main()
{
    char input[MAX_INPUT];
    char* argv[MAX_ARGS];

    printf("Для выхода из интерпретатора ввести: exit \n");

    while (1)
    {
        printf("Введите команду: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if(strcmp(input, "exit") == 0)
        {
            break;
        }

        if (strlen(input) == 0)
        {
            continue;
        }
        int argc = parse_input(input, argv);
        if (argc == 0)
        {
            continue;
        }

        execute_command(argv);
    }

    printf("Конец работы интерпритатора.\n");
    return 0;

    
}