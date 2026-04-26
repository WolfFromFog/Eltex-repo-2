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
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

// количество активных пользователей
int nclients;
fd_set master;
// Массив состояний для всех возможных сокетов
client_state_t client_state[FD_SETSIZE];

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void dostuff(int sock)
{
    client_state_t *st = &client_state[sock];
    char buff[1024];
    int n;
    struct sembuf decreas = {0, -1, 0};
    while (1)
    {
        if (st->state == STATE_GET_OP || st->state == STATE_GET_A || st->state == STATE_GET_B)
        {
            n = read(sock, buff, sizeof(buff) - 1);
            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    break;
                }
                else
                {
                    error("read error");
                }
            }
            else if (n == 0)
            {
                close(sock);
                FD_CLR(sock, &master);
                if (semop(nclients, &decreas, 1) == -1)
                {
                    perror("semop: decreas");
                }
                printf("-disconnect\n");
                printusers();
                return;
            }
            buff[n] = '\0';
        }
        switch (st->state)
        {
        case STATE_GET_OP:
            if (strstr(buff, "add"))
            {
                st->opr = 1;
            }

            if (strstr(buff, "sum"))
            {
                st->opr = 2;
            }

            if (strstr(buff, "mult"))
            {
                st->opr = 3;
            }

            if (strstr(buff, "div"))
            {
                st->opr = 4;
            }
            if (st->opr != 0)
            {
                st->state = STATE_GET_A;
                char msg[] = "Enter 1 parameter\r\n";
                write(sock, msg, strlen(msg));
            }
            else
            {
                char msg[] = "Wrong operation!\n";
                write(sock, msg, strlen(msg));
                st->state = STATE_QUIT;
            }
            break;

        case STATE_GET_A:
            st->a = atoi(buff);
            st->state = STATE_GET_B;
            char msg[] = "Enter 2 parameter\r\n";
            write(sock, msg, strlen(msg));
            break;

        case STATE_GET_B:
            st->b = atoi(buff);
            int result;
            switch (st->opr)
            {
            case 1:
                result = myadd(st->a, st->b);
                break;
            case 2:
                result = mysub(st->a, st->b);
                break;
            case 3:
                result = mymult(st->a, st->b);
                break;
            case 4:
                result = mydiv(st->a, st->b);
                break;

            default:
                break;
            }
            if (result != INT_MIN)
            {
                snprintf(st->out_buf, sizeof(st->out_buf), "%d", result);
            }
            else
            {
                strcpy(st->out_buf, "Error divison by zero");
            }
            st->out_len = strlen(st->out_buf);
            st->out_sent = 0;
            st->state = STATE_SEND_RES;
            // break;

        case STATE_SEND_RES:
            n = write(sock, st->out_buf + st->out_sent, st->out_len - st->out_sent);
            if (n == -1 && (errno != EAGAIN && errno != EWOULDBLOCK))
            {
                error("write error");
            }
            else if (n > 0)
            {
                st->out_sent += n;
                if (st->out_sent >= st->out_len)
                {
                    write(sock, "\nquit\n", 6);
                    st->state = STATE_QUIT;
                }
            }
            break;
        case STATE_QUIT:
            close(sock);
            FD_CLR(sock, &master);
            return;
        default:
            break;
        }
    }

    /*
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
    // nclients--; // уменьшаем счетчик активных клиентов
    printf("-disconnect\n");
    printusers();
    return;
    */
}

void printusers()
{
    int val = semctl(nclients, 0, GETVAL);
    if (val == -1)
    {
        perror("semctl GETVAL");
        return;
    }
    if (val > 0)
    {
        printf("%d user online\n", val);
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

void signaler(int sig)
{
    semctl(nclients, 0, IPC_RMID);
}

void set_nonblocking(int sock)
{
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1)
        error("fcntl F_GETFL");
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1)
        error("fcntl F_SETFL O_NONBLOCK");
}