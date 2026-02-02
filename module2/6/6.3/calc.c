#include "calc.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Размер массива хранящий функции
int f_array_size = 0;

//инициализация массива
operations* init_functions(const char* format)
{
	int len = strlen(format);

	operations* func_array = (operations*)malloc(len * sizeof(operations));

	if (func_array == NULL) {
		printf("Memmory allocation error!\n");
		return NULL;
	}
	int index = 0;

	for (int i = 0; i < len; i++)
	{
		switch (format[i]) 
		{
			case '1':
			{
				func_array[index++] = c_sum;
				break;
			}
			case '2':
			{
				func_array[index++] = c_sub;
				break;
			}
			case '3':
			{
				func_array[index++] = c_mult;
				break;
			}
			case '4':
			{
				func_array[index++] = c_div;
				break;
			}
			default:
				printf("Unknow function: %c!\n", format[i]);
		}
	}
	f_array_size = index;

	return func_array;
}

//Меню

void menu(const char* format)
{
	int choice = 0;
	double a, b;

	operations* func_array = init_functions(format);

	do
	{

		printf("\n--- CALCULATOR ---\n");

		for (int i = 0; i < f_array_size; i++)
		{
			switch (format[i])
			{
			case '1':
				printf("%d. Summation\n", i+1);
				break;
			case '2':
				printf("%d. Subtraction\n", i+1);
				break;
			case '3':
				printf("%d. Multiplication\n", i+1);
				break;
			case '4':
				printf("%d. Division\n", i+1);
				break;
			}
		}
		printf("0. Exit\n");
		printf("Choise operation: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("Enter a: ");
			scanf("%lf", &a);

			printf("Enter b: ");
			scanf("%lf", &b);
			func_array[choice-1](a,b);
			break;
		case 2:
			printf("Enter a: ");
			scanf("%lf", &a);

			printf("Enter b: ");
			scanf("%lf", &b);
			func_array[choice - 1](a, b);
			break;
		case 3:
			printf("Enter a: ");
			scanf("%lf", &a);

			printf("Enter b: ");
			scanf("%lf", &b);
			func_array[choice - 1](a, b);
			break;
		case 4:
			printf("Enter a: ");
			scanf("%lf", &a);

			printf("Enter b: ");
			scanf("%lf", &b);
			func_array[choice - 1](a, b);
			break;
		case 0:
			printf("\nExiting programm...\n");
			break;
		default:
			printf("\nWrong choise! Try again.\n");
		}

	} while (choice !=0);

	
}

//Математичесие функции
void c_sum(double a, double b)
{
	printf("\n%lf + %lf = %lf\n", a, b, a + b);
}
void c_sub(double a, double b)
{
	printf("\n%lf - %lf = %lf\n", a, b, a - b);
}
void c_mult(double a, double b)
{
	printf("\n%lf * %lf = %lf\n", a, b, a * b);
}
void c_div(double a, double b)
{
	if (b == 0)
	{
		printf("\nError: zero division\n");
		return;
	}
	printf("\n%lf / %lf = %lf\n", a, b, a / b);
}
