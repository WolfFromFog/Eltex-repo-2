#ifndef CHMOD_H
#define CHMOD_H
#include <sys/types.h>

char* rules(const char* rule_set);
char* rule_picker_char(char rule);
char* rule_picker_int(int rule);
void get_file_permission(const char* filename);
void print_file_permissions(mode_t mode);

#endif // !CHMOD_H

