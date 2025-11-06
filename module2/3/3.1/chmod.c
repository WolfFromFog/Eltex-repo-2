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
			printf("Wrong format!!!: %s", rule_set);
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

void get_file_permission(const char* filename)
{
	struct stat file_stat;

	if (stat(filename, &file_stat) == -1) {
		perror("Error reading file permissons");
		return;
	}

	printf("\n=== File permissons for: %s ===\n", filename);
        print_file_permissions(file_stat.st_mode);
        printf("==================================\n\n");

}

void print_file_permissions(mode_t mode)
{
    mode_t perm_bits = mode & 0777;
  
    int owner_perm = (perm_bits >> 6) & 7;
    int group_perm = (perm_bits >> 3) & 7;
    int other_perm = perm_bits & 7;
    
    char* owner_bits, group_bits, other_bits;
    
    owner_bits = rule_picker_int(owner_perm);
    group_bits = rule_picker_int(group_perm);
    other_bits = rule_picker_int(other_perm);
            
    //printf("Letter: %s\n", letters);
    printf("Numeric: %d%d%d\n", owner_perm, group_perm, other_perm);
    printf("Bit: %s%s%s\n", owner_bits, group_bits, other_bits);
    printf("Полное числовое значение: %04o\n", perm_bits);
    
}
