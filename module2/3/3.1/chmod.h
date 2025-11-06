#ifndef CHMOD_H
#define CHMOD_H
#include <sys/types.h>

char* rules(const char* rule_set);
char* rule_picker_char(char rule);
char* rule_picker_int(int rule);
mode_t get_file_permission(const char* filename);
void print_file_permissions(const char* filename);
void mode_to_letters(mode_t mode, char* str);
void modify_mode();
void ask_for_change();

#endif // !CHMOD_H

