#ifndef INTERPRETER_H

#define MAX_INPUT 500
#define MAX_ARGS 10
#define MAX_PIPE 10

//Структура, хранящая свойства для процесса

typedef struct command_s
{
    char *argv[MAX_ARGS];
    char *input_file;
    char *output_file;
    int pipe; 
} command_s;

//Парсинг аргументов
int parse_input(char *input, command_s commands[]);

//Проверка существования файла
int file_exist(char* filename);

//Поиск исполняемого файла
char *find_executable(char *command);

//Вызов исполняемого файла
void execute_pipline(command_s commands[], int cmd_num);
#endif 