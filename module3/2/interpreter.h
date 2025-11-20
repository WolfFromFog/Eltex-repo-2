#ifndef INTERPRETER_H

#define MAX_INPUT 50
#define MAX_ARGS 10

//Парсинг аргументов
int parse_input(char* input, char* argv[]);

//Проверка существования файла
int file_exist(char* filename);

//Поиск исполняемого файла
char *find_executable(char *command);

//Вызов исполняемого файла
void execute_command(char *argv[]);
#endif 