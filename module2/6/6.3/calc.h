#ifndef CALC_H
#define CALC_H

#define MAX_PLUGINS 20
#define PLUGIN_DIR "./plugins"

extern int plugin_count;

// Загрузка всех плагинов
void load_plugins(const char *dir_path);
// Выгрузка всех лагинов
void unload_plugins(void);
// Выполенение операции
double execute_operation(const char *symbol, double a, double b);
// Отображение списка операций
void print_operations(void);

#endif // !CALC_H
