#ifndef CALC_H
#define CALC_H

#define FORMAT_LEN 5

// ������ ���������� �� �������

typedef void (*operations)(double, double);

// ���������� ��� �������� ������� ������� ���������� �� �������

extern int f_array_size;

// ������������� ������� �� ������ �������

operations *init_functions(const char *format);

// ���� �� ������ �������

void menu(const char *format);

// �������������� �������

void c_sum(double a, double b);
void c_sub(double a, double b);
void c_mult(double a, double b);
void c_div(double a, double b);

#endif // !CALC_H
