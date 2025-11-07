#ifndef CHMOD_H
#define CHMOD_H
#include <sys/types.h>

char* rules(const char* rule_set);
char* rule_picker_char(char rule);
char* rule_picker_int(int rule);
mode_t get_file_permission(const char* filename);
void print_file_permissions(const char* filename);
void mode_to_letters(mode_t mode, char* str);
int modify_mode_temp(const char* rule_set);
int modify_mode_file(const char* filename);

#endif // !CHMOD_H

