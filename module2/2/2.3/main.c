#include <stdio.h> 
#include "calc.h"

int main() {
	//int choise = 0;
	char format[FORMAT_LEN];

	printf("Avaible functions: \n");
	printf("1. Summation \n");
	printf("2. Subtraction \n");
	printf("3. Multiplication \n");
	printf("4. Division \n");

	printf("Choose functions in format: 1234 or 134 or 1 for enable those functions.\n ");
	scanf("%4s", &format);

	
	
	/*for (int i = 0; i < f_array_size; i++)
	{
		func_array[i](2, 3);
	}*/

	menu(format);

	return 0;
}