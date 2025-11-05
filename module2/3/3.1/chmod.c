#include "chmod.h"
#include <string.h>
#include <stdio.h>

char* rules(const char* rule_set)
{
	int len = strlen(rule_set);
	static char str[10];

	if (len == 3)
	{
		strcpy(str, rule_picker(rule_set[0]));
		strcat(str, rule_picker(rule_set[1]));
		strcat(str, rule_picker(rule_set[2]));
		return &str;
	}
	else if (len == 9)
		{
		return "A";
		}
		else
		{
			printf("Wrong format!!!: %s", rule_set);
			return NULL;
		}
}

char* rule_picker(char rule)
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
