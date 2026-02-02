#ifndef CALC_H
#define CALC_H

#define FORMAT_LEN 5

//Массив указателей на функции

typedef void (*operations)(double, double);

//Переменная для хранения размера массива указателей на функции

extern int f_array_size;

//инициализация массива на основе формата

operations* init_functions(const char* format);

//Меню на основе массива

void menu(const char* format);

//Математические функции

void c_sum(double a, double b);
void c_sub(double a, double b);
void c_mult(double a, double b);
void c_div(double a, double b);



#endif // !CALC_H

