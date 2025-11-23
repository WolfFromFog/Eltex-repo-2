#include "interpreter.h"
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int parse_input(char *input, command_s commands[])
{
    for (int i = 0; i < MAX_PIPE; i++)
    {
        commands[i].input_file = NULL;
        commands[i].output_file = NULL;
        commands[i].pipe = 0;
        for (int j = 0; j < MAX_ARGS; j++)
        {
            commands[i].argv[j] = NULL;
        }
    }

        int cmd_counter = 0;
    int argc = 0;
    char input_copy[MAX_INPUT];
    // char *tokens[MAX_ARGS];
    char *pipe_pntr;

    // Копия входной строки для безопасного разбиения
    strncpy(input_copy, input, sizeof(input_copy));
    input_copy[sizeof(input_copy) - 1] = '\0';

    // Взятие первой команды
    char *big_token = __strtok_r(input_copy, "|", &pipe_pntr);

    // Парсинг конад внутри токена

    while (big_token != NULL)
    {

        // Убрать пробелы вначале токена
        while (*big_token == ' ')
            big_token++;

        // Убрать пробелы в конце токена
        char *token_end = big_token + strlen(big_token) - 1;
        while (token_end > big_token && *token_end == ' ')
            token_end--;
        *(token_end + 1) = '\0';

        argc = 0;
        char cmd_copy[MAX_INPUT];
        char *cmd_pntr;

        strncpy(cmd_copy, big_token, sizeof(cmd_copy));
        cmd_copy[sizeof(cmd_copy) - 1] = '\0';

        char *small_token = __strtok_r(cmd_copy, " \t", &cmd_pntr);

        while (small_token != NULL)
        {
            if (strcmp(small_token, "<") == 0)
            {
                small_token = __strtok_r(NULL, " \t", &cmd_pntr);
                if (small_token != NULL)
                {
                    commands[cmd_counter].input_file = malloc(sizeof(small_token) + 1);
                    strcpy(commands[cmd_counter].input_file, small_token);
                }
            }
            else if (strcmp(small_token, ">") == 0)
            {
                small_token = __strtok_r(NULL, " \t", &cmd_pntr);
                if (small_token != NULL)
                {
                    commands[cmd_counter].output_file = malloc(sizeof(small_token) + 1);
                    strcpy(commands[cmd_counter].output_file, small_token);
                }
            }
            else
            {
                commands[cmd_counter].argv[argc] = malloc(sizeof(small_token) + 1);
                strcpy(commands[cmd_counter].argv[argc++], small_token);
            }

            small_token = __strtok_r(NULL, " \t", &cmd_pntr);
            if (cmd_counter > 0)
            {
                commands[cmd_counter - 1].pipe = 1;
            }
        }
        cmd_counter++;

        big_token = __strtok_r(NULL, "|", &pipe_pntr);
    }
    // argv[argc]=NULL;

    return cmd_counter;
}

int file_exist(char *filename)
{
    struct stat st;
    return (stat(filename, &st) == 0);
}

char *find_executable(char *command)
{

    char full_path[1024];
    // Абсолютный путь
    if (strchr(command, '/'))
    {
        if (file_exist(command))
        {
            return command;
        }
    }

    // В текущем каталоге
    snprintf(full_path, sizeof(full_path), "./%s", command);
    if (file_exist(full_path))
    {
        return strdup(full_path);
    }
    // Системные
    char *path = getenv("PATH");
    if (path == NULL)
    {
        return NULL;
    }
    // Копия, чтоб не поломать системную перменнную
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");

    // Перебираем все пути систмных переменных, пока не будет совпадения или все не переберём
    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (file_exist(full_path))
        {
            free(path_copy);
            return strdup(full_path);
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);

    return NULL;
}

void execute_pipline(command_s commands[], int cmd_num)
{
    /*
    if (commands == NULL)
    {
        return;
    }

    pid_t pid = fork();


    if( pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0){
        char *exec_path = find_executable(argv[0]);

        if (exec_path == NULL)
        {
            printf("Команда не найдена: %s\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        execv(exec_path, argv);

        perror("execv");
        exit(EXIT_FAILURE);
    }

    else{
        int status;
        waitpid(pid, &status, 0);
    }

    */
}
