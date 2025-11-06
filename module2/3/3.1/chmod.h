#ifndef CHMOD_H
#define CHMOD_H

char* rules(const char* rule_set);
const char* rule_picker(char rule);
void get_file_permission(const char* filename);
void print_file_permissions(mode_t mode);

#endif // !CHMOD_H

