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
#include <sys/stat.h>

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

    buff[strcspn(buff, "\r\n")] = 0;

    if (strcmp(buff, "file") == 0)
    {
        write(sock, "FILE_MODE_READY\n", 16);
        dostuff_file(sock);
        if (semop(nclients, &decreas, 1) == -1)
        {
            perror("semop: decreas");
        }
        return;
    }

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

int receive_file(int sock, const char *filename)
{
    char buff[4096];
    int bytes_received;
    FILE *file = fopen(filename, "wb");

    if (file == NULL)
    {
        perror("Error fopen for writeing");
        return -1;
    }

    // Получение размера файла
    long file_size;
    bytes_received = recv(sock, &file_size, sizeof(file_size), 0);
    if (bytes_received <= 0)
    {
        perror("Error receiving file size");
        fclose(file);
        return -1;
    }

    send(sock, "OK", 2, 0);
    // Чтение файла
    long total_received = 0;
    while (total_received < file_size)
    {
        bytes_received = recv(sock, buff, sizeof(buff), 0);
        if (bytes_received <= 0)
        {
            perror("Error receiving file data");
            fclose(file);
            return -1;
        }
        fwrite(buff, 1, bytes_received, file);
        total_received += bytes_received;
    }
    fclose(file);
    printf("Файл получен: %s (размер %ld байт)\n", filename, file_size);
    return 0;
}

int send_file(int sock, const char *filename)
{
    char buff[4096];
    int bytes_read;
    struct stat file_stat;
    FILE *file = fopen(filename, "rb");

    if (file == NULL)
    {
        perror("Error fopen for reading");
        return -1;
    }
    // Получение парамтеров файла
    if (stat(filename, &file_stat) != 0)
    {
        perror("Error getting file stats");
        fclose(file);
        return -1;
    }

    long file_size = file_stat.st_size;

    send(sock, &file_size, sizeof(file_size), 0);

    // Ожидание "OK"
    char ack[3];
    recv(sock, ack, 2, 0);

    // Отправка файла
    long total_sent = 0;
    while ((bytes_read = fread(buff, 1, sizeof(buff), file)) > 0)
    {
        send(sock, buff, bytes_read, 0);
        total_sent += bytes_read;
    }
    fclose(file);
    printf("Файл отправлен: %s (размер %ld байт)\n", filename, file_size);
    return 0;
}

void dostuff_file(int sock)
{
    char filename_in[] = "client_request.txt";
    char filename_out[] = "server_response.txt";

    if (receive_file(sock, filename_in) < 0)
    {
        write(sock, "ERROR: Failed to receive file\n", 30);
        return;
    }
    // Приём файла
    FILE *file = fopen(filename_in, "r");
    if (file == NULL)
    {
        write(sock, "ERROR: Cannot read input file\n", 30);
        return;
    }

    char operation[50];
    int a, b;

    // Чтение операции и операндов
    if (fgets(operation, sizeof(operation), file) == NULL)
    {
        write(sock, "ERROR: Invalid file format\n", 27);
        fclose(file);
        return;
    }

    operation[strcspn(operation, "\n")] = 0;

    if (fscanf(file, "%d %d", &a, &b) != 2)
    {
        write(sock, "ERROR: Invalid operands format\n", 31);
        fclose(file);
        return;
    }

    fclose(file);

    int result;
    char result_str[100];

    if (strcmp(operation, "add") == 0)
    {
        result = myadd(a, b);
        snprintf(result_str, sizeof(result_str), "%d", result);
    }
    else if (strcmp(operation, "sub") == 0)
    {
        result = mysub(a, b);
        snprintf(result_str, sizeof(result_str), "%d", result);
    }
    else if (strcmp(operation, "mult") == 0)
    {
        result = mymult(a, b);
        snprintf(result_str, sizeof(result_str), "%d", result);
    }
    else if (strcmp(operation, "div") == 0)
    {
        result = mydiv(a, b);
        if (result != INT_MIN)
        {
            snprintf(result_str, sizeof(result_str), "%d", result);
        }
        else
        {
            strcpy(result_str, "Error: Division by zero");
        }
    }
    else
    {
        strcpy(result_str, "Error: Unknown operation");
    }

    // Ответ
    FILE *outfile = fopen(filename_out, "w");
    if (outfile == NULL)
    {
        write(sock, "ERROR: Cannot create output file\n", 33);
        return;
    }

    fprintf(outfile, "Operation: %s\n", operation);
    fprintf(outfile, "Operand 1: %d\n", a);
    fprintf(outfile, "Operand 2: %d\n", b);
    fprintf(outfile, "Result: %s\n", result_str);
    fclose(outfile);

    // Отправляем файл
    if (send_file(sock, filename_out) < 0)
    {
        write(sock, "ERROR: Failed to send response file\n", 36);
        return;
    }

    // Отправляем подтверждение завершения
    write(sock, "File processing completed\n", 26);

    unlink(filename_in);
    unlink(filename_out);

    printf("-disconnect (file mode)\n");
    printusers();
}
