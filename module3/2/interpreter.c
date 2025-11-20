#include "interpreter.h"
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int parse_input(char *input, char *argv[])
{
    int argc = 0;

    //Взятие превого аргумента
    char* token = strtok(input, " \t\n");
    
    while (token != NULL)
    {
       
        argv[argc++] = token;
        // Взятие последующих аргументов
        token = strtok(NULL, " \t\n");
    }
    argv[argc]=NULL;

    return argc;
}

int file_exist(char *filename)
{
    struct stat st;
    return (stat(filename, &st) == 0);
}

char *find_executable(char *command)
{
    
    char full_path[1024];
    //Абсолютный путь
    if(strchr(command, '/'))
    {
        if (file_exist(command))
        {
            return command;
        }
    }

    //В текущем каталоге
    snprintf(full_path, sizeof(full_path), "./%s", command);
    if (file_exist(full_path))
    {
        return full_path;
    }
    //Системные
    char* path = getenv("PATH");
    if (path == NULL)
    {
        return NULL;
    }
    //Копия, чтоб не поломать системную перменнную
    char* path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");

    //Перебираем все пути систмных переменных, пока не будет совпадения или все не переберём
    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (file_exist(full_path))
        {
            free(path_copy);
            return full_path;
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    
    return NULL;
}

void execute_command(char *argv[])
{
    pid_t pid;

    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;
    case 0:
        char *exec_path = find_executable(argv[0]);

        if (exec_path == NULL)
        {
            printf("Команда не найдена: %s\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        execv(exec_path, argv);

        perror("execv");
        exit(EXIT_FAILURE);

    default:
        int status;
        waitpid(pid, &status, 0);
        break;
    }
}
