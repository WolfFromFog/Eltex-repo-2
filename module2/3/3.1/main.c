#include <stdio.h> 
#include "chmod.h"
#include <string.h>



int main(int argc, char* argv[])
{
	const char* ch = "rwx------";

	//char result[10];

	const char* result = rules(ch);
	printf("Result is: %s\n", result);
	
	if (argc > 1) {
          for (int i = 1; i < argc; i++) {
              get_file_permission(argv[i]);
          } 
        } else {
          
          get_file_permission(".");
          get_file_permission("chmod.c");
          get_file_permission("main.c");
          }

	return 0;
}
