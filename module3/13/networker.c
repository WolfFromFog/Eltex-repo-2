#include "networker.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// количество активных пользователей
int nclients = 0;

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_SUCCESS);
}

void dostuff(int sock)
{
    int bytes_recv; // размер принятого сообщения
    int a, b;       // переменные для myfunc
    char buff[20 * 1024];
#define str1 "Enter 1 parameter\r\n"
#define str2 "Enter 2 parameter\r\n"

    // отправляем клиенту сообщение
    write(sock, str1, sizeof(str1));

    // обработка первого параметра
    bytes_recv = read(sock, buff, sizeof(buff));
    if (bytes_recv < 0)
        error("ERROR reading from socket");
    a = atoi(buff); // преобразование первого параметра в int

    // отправляем клиенту сообщение
    write(sock, str2, sizeof(str2));
    bytes_recv = read(sock, buff, sizeof(buff));
    if (bytes_recv < 0)
        error("ERROR reading from socket");
    b = atoi(buff); // преобразование второго параметра в int

    a = myfunc(a, b);                      // вызов пользовательской функции
    snprintf(buff, strlen(buff), "%d", a); // преобразование результата в строку
    buff[strlen(buff)] = '\n';             // добавление к сообщению символа конца строки
    // отправляем клиенту результат
    write(sock, &buff[0], sizeof(buff));
    nclients--; // уменьшаем счетчик активных клиентов
    printf("-disconnect\n");
    printusers();
    return;
}

void printusers()
{
    if (nclients)
    {
        printf("%d user online\n", nclients);
    }
    else
    {
        printf("No users online\n");
    }
}

// функция обработки данных
int myfunc(int a, int b)
{
    return a + b;
}