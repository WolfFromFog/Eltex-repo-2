#include <stdio.h> 
#include "chmod.h"
#include <string.h>



int main(int argc, char* argv[])
{ 
        int choice = 0;
        char choice_c = '0';
        if(argc == 1)
        {
          printf("Выберите режим:\n");
          printf("1 - Ввод правил в консоль, вывод их бинарной формы\n");
          printf("2 - Вывод правил файла:\n");
          
          scanf("%d",&choice);
          
          if(choice == 1)
          {
            char rule_set[10];
            printf("Введи набор правил в консоль в формате \"777\" или \"rwxrwxrwx\": ");
            scanf("%s",&rule_set);
            const char* result = rules(rule_set);
	    printf("Правила: %s\n", result);
            printf("Хотите изменить правила? (Y/n): ");
            scanf(" %c",&choice_c);
            if(choice_c == 'Y' || choice_c == 'y') modify_mode_temp(result);
            
          }
          else if (choice == 2)
               {
                  char filename[30];
                  printf("Ввеждите имя файла для отображения его правил: ");
                  scanf("%s",&filename);
                  print_file_permissions(filename);
                  printf("Хотите изменить правила? (Y/n): ");
                  scanf(" %c",&choice_c);
                  if(choice_c == 'Y' || choice_c == 'y') modify_mode_file(filename);
               }
          else
          {
            printf("Выбрать неправильный режим. Закрытие \n");
            return -1;
          }
        }
        /*
	const char* ch = "rwx------";

	char ruleset = argv[1];

	const char* result = rules(ch);
	printf("Result is: %s\n", result);
	
	if (argc > 1) {
          for (int i = 1; i < argc; i++) {
              print_file_permissions(argv[i]);
          } 
        } else {
          
          print_file_permissions(".");
          print_file_permissions("chmod.c");
          print_file_permissions("chmod.h");
          print_file_permissions("main.c");
          print_file_permissions("Makefile");
          print_file_permissions("cast_chmod");
          }
        */
	return 0;
}
