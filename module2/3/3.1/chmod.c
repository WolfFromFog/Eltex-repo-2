#include "chmod.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

char* rules(const char* rule_set)
{
	int len = strlen(rule_set);
	static char str[10];

	if (len == 3)
	{
		strcpy(str, rule_picker_char(rule_set[0]));
		strcat(str, rule_picker_char(rule_set[1]));
		strcat(str, rule_picker_char(rule_set[2]));
		return str;
	}
	else if (len == 9)
		{
		//str[0] = rule_set[0] == 'r' ? '1' : '0';
		for (int i = 0; i < len; i++)
		{
			if (i % 3 == 0) str[i] = rule_set[i] == 'r' ? '1' : '0';
			if (i % 3 == 1) str[i] = rule_set[i] == 'w' ? '1' : '0';
			if (i % 3 == 2) str[i] = rule_set[i] == 'x' ? '1' : '0';
		}
		return str;
		}
		else
		{
			printf("Wrong format!!!: %s \n", rule_set);
			return NULL;
		}
}

char* rule_picker_char(char rule)
{
	switch (rule)
	{
	case '0':
		return "000";
		break;
	case '1':
		return "001";
		break;
	case '2':
		return "010";
		break;
	case '3':
		return "011";
		break;
	case '4':
		return "100";
		break;
	case '5':
		return "101";
		break;
	case '6':
		return "110";
		break;
	case '7':
		return "111";
		break;
	default:
		printf("Wrong rule!!!: %c", rule);
		return NULL;
		break;
	}
}

 char* rule_picker_int(int rule)
{
	switch (rule)
	{
	case 0:
		return "000";
		break;
	case 1:
		return "001";
		break;
	case 2:
		return "010";
		break;
	case 3:
		return "011";
		break;
	case 4:
		return "100";
		break;
	case 5:
		return "101";
		break;
	case 6:
		return "110";
		break;
	case 7:
		return "111";
		break;
	default:
		printf("Wrong rule!!!: %c", rule);
		return NULL;
		break;
	}
}
void mode_to_letters(mode_t mode, char* str)
{
   // Права пользователя
    str[0] = (mode & S_IRUSR) ? 'r' : '-';
    str[1] = (mode & S_IWUSR) ? 'w' : '-';
    str[2] = (mode & S_IXUSR) ? 'x' : '-';

    // Права группы
    str[3] = (mode & S_IRGRP) ? 'r' : '-';
    str[4] = (mode & S_IWGRP) ? 'w' : '-';
    str[5] = (mode & S_IXGRP) ? 'x' : '-';

    // Права других
    str[6] = (mode & S_IROTH) ? 'r' : '-';
    str[7] = (mode & S_IWOTH) ? 'w' : '-';
    str[8] = (mode & S_IXOTH) ? 'x' : '-';
    
    str[9] = '\0';
}
mode_t get_file_permission(const char* filename)
{
	struct stat file_stat;

	if (stat(filename, &file_stat) == -1) {
		perror("Error reading file permissons");
		return -9999;
	}
        return file_stat.st_mode;
	//printf("\n=== File permissons for: %s ===\n", filename);
        //print_file_permissions(file_stat.st_mode);
        //printf("==================================\n\n");
  
}

void print_file_permissions(const char* filename)
{
    
    mode_t mode = get_file_permission(filename);
    
    char letters[10];
    mode_to_letters(mode, letters);
    
    mode_t perm_bits = mode & 0777;
  
    int owner_perm = (perm_bits >> 6) & 7;
    int group_perm = (perm_bits >> 3) & 7;
    int other_perm = perm_bits & 7;
    
    
    
    const char* owner_bits = rule_picker_int(owner_perm);
    const char* group_bits = rule_picker_int(group_perm);
    const char* other_bits = rule_picker_int(other_perm);
    
    printf("\n=== File permissons for: %s ===\n", filename);      
    printf("Letter: %s\n", letters);
    printf("Numeric: %d%d%d\n", owner_perm, group_perm, other_perm);
    printf("Bit: %s%s%s\n", owner_bits, group_bits, other_bits);
    printf("==================================\n\n");
}

int symbolic_mode_parser(const char* mode_str, mode_t current_mode, mode_t* new_mode)
{
	const char* p = mode_str;
	mode_t who = 0; 
	mode_t perm = 0;
	char operation = '\0';

	*new_mode = current_mode;

	while(*p)
	{
		//Определяем кому менять
		who = 0;
		while(*p)
		{
			switch(*p)
			{
				case 'u': who |=S_IRWXU; break;
				case 'g': who |=S_IRWXG; break;
				case 'o': who |=S_IRWXO; break;
				case 'a': who |=S_IRWXU | S_IRWXG | S_IRWXO; break;
				default: goto parser_operation;
			}
			p++;
		}
		// Определяем что менять
		parser_operation:

		if(*p == '\0') return -1;
		operation = *p++;

		while(*p)
		{
			switch(*p)
			{
				case 'r': perm |= S_IRUSR | S_IRGRP | S_IROTH; break;
				case 'w': perm |= S_IWUSR | S_IWGRP | S_IWOTH; break; 
				case 'x': perm |= S_IXUSR | S_IXGRP | S_IXOTH; break; 
				case '\0': goto apply_mode; 
			}
			p++;
		}
		apply_mode:
		switch(operation)
		{
			case '+': *new_mode |= (perm & who); break;
			case '-': *new_mode &= ~(perm & who); break;
			case '=': *new_mode = (perm & who); break;
			default: return -1;
		}
	}
	
	return 0;
}

int modify_mode_temp(const char* rule_set)
{
    char format[9];
    printf("Введите правила в формате XYZ, где X - кому, Y - условие, Z - правила\n");
    printf("X = ugoa\n Y = +-= \n Z = rwx\n");
    scanf("%8s",&format);
	mode_t current_mode = 0;
	if(strlen(rule_set) == 9)
	{
		for(int i=0; i<9;i ++)
		{
			if(rule_set[i] == '1')
			{
				switch (i) {
                    case 0: current_mode |= S_IRUSR; break;
                    case 1: current_mode |= S_IWUSR; break;
                    case 2: current_mode |= S_IXUSR; break;
                    case 3: current_mode |= S_IRGRP; break;
                    case 4: current_mode |= S_IWGRP; break;
                    case 5: current_mode |= S_IXGRP; break;
                    case 6: current_mode |= S_IROTH; break;
                    case 7: current_mode |= S_IWOTH; break;
                    case 8: current_mode |= S_IXOTH; break;
                }
			}
		}
	}
	
	mode_t new_mode;
	if(symbolic_mode_parser(format, current_mode, &new_mode) == 0)
	{

		char letters[10];
    	mode_to_letters(new_mode, letters);

		mode_t perm_bits = new_mode & 0777;
  
		int owner_perm = (perm_bits >> 6) & 7;
		int group_perm = (perm_bits >> 3) & 7;
		int other_perm = perm_bits & 7;
		
		
		
		const char* owner_bits = rule_picker_int(owner_perm);
		const char* group_bits = rule_picker_int(group_perm);
		const char* other_bits = rule_picker_int(other_perm);
		
		printf("\n=== New permissons for: %s ===\n", rule_set);      
		printf("Letter: %s\n", letters);
		printf("Numeric: %d%d%d\n", owner_perm, group_perm, other_perm);
		printf("Bit: %s%s%s\n", owner_bits, group_bits, other_bits);
		printf("==================================\n\n");
		return 0;
	}
	else
	{
		printf("Ошибка при парсинге: неправильный формат\n");
		return -1;
	} 
}
int modify_mode_file(const char* filename)
{
    char format[9];
    printf("Введите правила в формате XYZ, где X - кому, Y - условие, Z - правила\n");
    printf("X = ugoa\n Y = +-= \n Z = rwx\n");
    scanf("%8s",&format);
	mode_t current_mode = get_file_permission(filename);

	mode_t new_mode ;
	if(symbolic_mode_parser(format, current_mode, &new_mode) == 0)
	{

		char letters[10];
    	mode_to_letters(new_mode, letters);

		mode_t perm_bits = new_mode & 0777;
  
		int owner_perm = (perm_bits >> 6) & 7;
		int group_perm = (perm_bits >> 3) & 7;
		int other_perm = perm_bits & 7;
		
		
		
		const char* owner_bits = rule_picker_int(owner_perm);
		const char* group_bits = rule_picker_int(group_perm);
		const char* other_bits = rule_picker_int(other_perm);
		
		printf("\n=== New permissons for: %s ===\n", filename);      
		printf("Letter: %s\n", letters);
		printf("Numeric: %d%d%d\n", owner_perm, group_perm, other_perm);
		printf("Bit: %s%s%s\n", owner_bits, group_bits, other_bits);
		printf("==================================\n\n");
		return 0;
	}
	else 
	{
		printf("Ошибка при парсинге: неправильный формат\n");
		return -1;
	} 
}


