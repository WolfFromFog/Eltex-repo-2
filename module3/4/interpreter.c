#include "interpreter.h"
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int isFileEmpty(int filedescriptor)
{
    off_t cur_pos = lseek(filedescriptor, 0, SEEK_CUR);
    off_t file_size = lseek(filedescriptor, 0, SEEK_END);
    lseek(filedescriptor, cur_pos, SEEK_SET);
    return file_size == 0;
}

int parse_input(char *input, command_s commands[])
{
    for (int i = 0; i < MAX_PIPES; i++)
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
                    commands[cmd_counter].input_file = malloc(strlen(small_token) + 1);
                    strcpy(commands[cmd_counter].input_file, small_token);
                }
            }
            else if (strcmp(small_token, ">") == 0)
            {
                small_token = __strtok_r(NULL, " \t", &cmd_pntr);
                if (small_token != NULL)
                {
                    commands[cmd_counter].output_file = malloc(strlen(small_token) + 1);
                    strcpy(commands[cmd_counter].output_file, small_token);
                }
            }
            else
            {
                commands[cmd_counter].argv[argc] = malloc(strlen(small_token) + 1);
                strcpy(commands[cmd_counter].argv[argc++], small_token);
            }
            small_token = __strtok_r(NULL, " \t", &cmd_pntr);
        }
        commands[cmd_counter].argv[argc] = NULL;

        if (cmd_counter > 0)
        {
            commands[cmd_counter - 1].pipe = 1;
        }

        cmd_counter++;
        big_token = __strtok_r(NULL, "|", &pipe_pntr);
    }

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
    int pipe_fd[2 * (MAX_PIPES - 1)];
    pid_t pids[MAX_PIPES];
    // pipe()

    if (commands == NULL)
    {
        fprintf(stderr, "Нету команд\n");
        return;
    }

    for (int i = 0; i < cmd_num - 1; i++)
    {
        int p_id = pipe(pipe_fd + i * 2);
        if (p_id < 0)
        {
            perror("pipe");
            return;
        }
    }

    for (int i = 0; i < cmd_num; i++)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            fprintf(stderr, "Fork failed \n");
            perror("fork");
            return;
        }
        if (pids[i] == 0)
        {
            // Дочерний
            if (commands[i].input_file != NULL)
            {
                int fd_in = open(commands[i].input_file, O_RDONLY);
                if (fd_in < 0)
                {
                    fprintf(stderr, "Нету такого файла для чтения или не удалось открыть: %s\n", commands[i].input_file);
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                if (isFileEmpty(fd_in))
                {
                    fprintf(stderr, "Файл пуст. \n");
                    perror(commands[i].input_file);
                    exit(EXIT_FAILURE);
                }
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
            // Перенарвление вывода
            else if (i > 0)
            {
                dup2(pipe_fd[(i - 1) * 2], STDIN_FILENO);
            }
            if (commands[i].output_file != NULL)
            {
                int fd_out = open(commands[i].output_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
                fprintf(stderr, "Создан или открыт на запись файл: %s\n", commands[i].output_file);
                if (fd_out < 0)
                {
                    fprintf(stderr, "Нету такого файла/ не удалось создать \n");
                    perror(commands[i].output_file);
                    exit(EXIT_FAILURE);
                }
                fprintf(stderr, "Дескриптор файла %d\n", fd_out);
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }
            else if (i < cmd_num - 1)
            {
                dup2(pipe_fd[i * 2 + 1], STDOUT_FILENO);
            }
            for (int j = 0; j < 2 * (cmd_num - 1); j++)
            {
                close(pipe_fd[j]);
            }
            char *exec_path = find_executable(commands[i].argv[0]);

            if (exec_path == NULL)
            {
                fprintf(stderr, "Команда не найдена: %s\n", commands[i].argv[0]);
                // perror(commands[i].argv[0]);
                exit(EXIT_FAILURE);
            }

            execv(exec_path, &commands[i].argv[0]);
            // printf("!!!!\n");
            perror("execv");
            free(exec_path);
            exit(EXIT_FAILURE);
        }
    }
    // Родительский
    for (int j = 0; j < 2 * (cmd_num - 1); j++)
    {
        close(pipe_fd[j]);
    }

    for (int i = 0; i < cmd_num; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
    }

    for (int i = 0; i < MAX_PIPES; i++)
    {
        free(commands[i].input_file);
        free(commands[i].output_file);
        for (int j = 0; j < MAX_ARGS; j++)
        {
            free(commands[i].argv[j]);
        }
    }
    return;
}
