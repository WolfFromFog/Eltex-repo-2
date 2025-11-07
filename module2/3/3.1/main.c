#include <stdio.h> 
#include "chmod.h"
#include <string.h>



int main(int argc, char* argv[])
{ 
        int choice = 0;
        char choice_c = '0';
        int Error = 0;
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
            if(result == NULL)
            {
              return -1;
            }
	          printf("Правила: %s\n", result);
            printf("Хотите изменить правила? (Y/n): ");
            scanf(" %c",&choice_c);
            if(choice_c == 'Y' || choice_c == 'y') Error = modify_mode_temp(result);
            if(Error != 0)
            return -1;
            
          }
          else if (choice == 2)
               {
                  char filename[30];
                  printf("Ввеждите имя файла для отображения его правил: ");
                  scanf("%s",&filename);
                  print_file_permissions(filename);
                  printf("Хотите изменить правила? (Y/n): ");
                  scanf(" %c",&choice_c);
                  if(choice_c == 'Y' || choice_c == 'y') Error = modify_mode_file(filename);
                  if(Error != 0)
                  return -1;
               }
          else
          {
            printf("Выбран неправильный режим. Закрытие \n");
            return -1;
          }
        }
	return 0;
}
