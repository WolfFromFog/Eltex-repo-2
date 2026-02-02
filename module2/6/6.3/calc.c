#include "calc.h"
#include "plugin_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>

typedef struct
{
	void *handle;
	PluginOperation *operation;
} LoadedPlugin;

LoadedPlugin plugins[MAX_PLUGINS];
int plugin_count = 0;

void load_plugins(const char *dir_path)
{
	DIR *dir = opendir(dir_path);
	if (!dir)
	{
		perror("Не удалаось открыть дирректорию с плагинами.");
		return;
	}

	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL && plugin_count < MAX_PLUGINS)
	{
		if (entry->d_name[0] == '.' || strstr(entry->d_name, ".so") == NULL)
		{
			continue;
		}

		char plugin_path[512];

		snprintf(plugin_path, sizeof(plugin_path), "%s/%s", dir_path, entry->d_name);

		printf("Загрузка плагина: %s\n", entry->d_name);

		// Загрузка библиотеки
		void *handle = dlopen(plugin_path, RTLD_LAZY);

		if (!handle)
		{
			fprintf(stderr, "Ошибка загрузки плагина %s: %s\n", entry->d_name, dlerror());
			continue;
		}
		// Получение функции регистрации
		register_plugin_function register_func = (register_plugin_function)dlsym(handle, "register_plugin");

		if (!register_func)
		{
			fprintf(stderr, "Ошибка нахождения register_plugin для регистрации плагина %s: %s\n", entry->d_name, dlerror());
			dlclose(handle);
			continue;
		}

		// Регистрация
		PluginOperation *op = register_func();

		if (!op)
		{
			fprintf(stderr, "Ошибка регистрации плагина %s\n", entry->d_name);
			dlclose(handle);
			continue;
		}

		// Сохранение плагина
		plugins[plugin_count].handle = handle;
		plugins[plugin_count++].operation = op;

		printf("Загрузка плагина %s завершена. Символ операции: %s.\n", op->name, op->symbol);
	}
	closedir(dir);
}

void unload_plugins(void)
{
	for (int i = 0; i < plugin_count; i++)
	{
		if (plugins[i].handle)
		{
			dlclose(plugins[i].handle);
		}
	}
	plugin_count = 0;
}

double execute_operation(const char *symbol, double a, double b)
{
	for (int i = 0; i < plugin_count; i++)
	{
		if (strcmp(plugins[i].operation->symbol, symbol) == 0)
		{
			return plugins[i].operation->operation(a, b);
		}
	}
	fprintf(stderr, "Операция '%s' не найдена", symbol);
	return 0;
}

void print_operations(void)
{
	printf("Доступные операции: \n");
	for (int i = 0; i < plugin_count; i++)
	{
		printf("%d. %s (%s);\n", i, plugins[i].operation->name, plugins[i].operation->symbol);
	}
}