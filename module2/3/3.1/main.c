#include <stdio.h> 
#include "chmod.h"
#include <string.h>



int main()
{
	const char* ch = "777";

	char result[10];

	strcpy(result, rules(ch));

	printf("Result is: %s", result);

	return 0;
}
