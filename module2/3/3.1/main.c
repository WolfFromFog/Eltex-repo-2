#include <stdio.h> 
#include "chmod.h"
#include <string.h>



int main()
{
	const char* ch = "rwx------";

	char result[10];

	strcpy(result, rules(ch));

	printf("Result is: %s\n", result);

	return 0;
}