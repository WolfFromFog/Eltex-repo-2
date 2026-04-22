#include "networker.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <limits.h>
#include <sys/sem.h>

// количество активных пользователей
int nclients;

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void dostuff(int sock)
{
    int bytes_recv;    // размер принятого сообщения
    int a, b, opr = 0; // переменные для myfunc
    char buff[20 * 1024];

    // struct sembuf increas = {0, 1, 0};
    struct sembuf decreas = {0, -1, 0};

#define str0 "Enter operation: add/sum/mult/div\r\n"
#define str1 "Enter 1 parameter\r\n"
#define str2 "Enter 2 parameter\r\n"

    write(sock, str0, strlen(str0));

    // Обработка операции
    bytes_recv = read(sock, buff, sizeof(buff));
    if (bytes_recv < 0)
        error("ERROR reading from socket");

    buff[bytes_recv] = '\0';

    if (strstr(buff, "add"))
    {
        opr = 1;
    }

    if (strstr(buff, "sum"))
    {
        opr = 2;
    }

    if (strstr(buff, "mult"))
    {
        opr = 3;
    }

    if (strstr(buff, "div"))
    {
        opr = 4;
    }

    if (opr == 0)
    {
        printf("Wrong operation!");
    }

    // отправляем клиенту сообщение
    write(sock, str1, strlen(str1));

    // обработка первого параметра
    bytes_recv = read(sock, buff, sizeof(buff));
    if (bytes_recv < 0)
        error("ERROR reading from socket");
    buff[bytes_recv] = '\0';
    a = atoi(buff); // преобразование первого параметра в int

    // отправляем клиенту сообщение
    write(sock, str2, strlen(str2));
    bytes_recv = read(sock, buff, sizeof(buff));
    if (bytes_recv < 0)
        error("ERROR reading from socket");
    buff[bytes_recv] = '\0';
    b = atoi(buff); // преобразование второго параметра в int

    // вызов пользовательской функции
    // memset(buff, 0, sizeof(buff));
    int len = 0;
    switch (opr)
    {
    case 1:
        a = myadd(a, b);
        len = snprintf(buff, sizeof(buff), "%d", a);
        break;
    case 2:
        a = mysub(a, b);
        len = snprintf(buff, sizeof(buff), "%d", a);
        break;
    case 3:
        a = mymult(a, b);
        len = snprintf(buff, sizeof(buff), "%d", a);
        break;
    case 4:
        a = mydiv(a, b);
        if (a != INT_MIN)
        {
            len = snprintf(buff, sizeof(buff), "%d", a);
        }
        else
        {
            strcpy(buff, "Error divison by zero");
        }

        break;

    default:
        break;
    }

    // buff[strlen(buff)] = '\n'; // добавление к сообщению символа конца строки
    //  отправляем клиенту результат
    if (len < 0 || len >= sizeof(buff))
    {
        printf("Error\n");
    }
    else
    {
        write(sock, buff, strlen(buff));
    }
    sleep(0.5);
    strcpy(buff, "quit\n");
    write(sock, buff, strlen(buff));
    if (semop(nclients, &decreas, 1) == -1)
    {
        perror("semop: decreas");
    }
    nclients--; // уменьшаем счетчик активных клиентов
    printf("-disconnect\n");
    printusers();
    return;
}

void printusers()
{
    if (nclients > 0)
    {
        printf("%d user online\n", nclients);
    }
    else
    {
        printf("No users online\n");
    }
}

// сумма
int myadd(int a, int b)
{
    return a + b;
}

int mysub(int a, int b)
{
    return a - b;
}

int mymult(int a, int b)
{
    return a * b;
}

int mydiv(int a, int b)
{
    if (b == 0)
    {
        printf("Division on zero\n");
        perror("Division on zero");
        return INT_MIN;
    }

    return a / b;
}
