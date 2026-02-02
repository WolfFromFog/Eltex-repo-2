#include <stdio.h>
#include "calc.h"

int main()
{

	load_plugins(PLUGIN_DIR);

	if (plugin_count == 0)
	{
		printf("Плагины не обнаружены.\n");
		return -1;
	}

	print_operations();
	printf("\n");

	double a, b;

	char op[10];

	printf("Введите выражение (например, 5 + 3): ");
	while (scanf("%lf %s %lf", &a, op, &b) == 3)
	{
		double result = execute_operation(op, a, b);
		printf("Результат: %lf %s %lf = %f\n\n", a, op, b, result);
		printf("Введите выражение (или Ctrl+D для выхода): ");
	}

	// Выгружаем плагины
	unload_plugins();
	return 0;
}