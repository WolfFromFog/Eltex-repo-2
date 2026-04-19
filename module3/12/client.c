#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include "networker.h"
#include <signal.h>

int main(int argc, char *argv[])
{
    // int sockfd;                           /* Дескриптор сокета */
    int n, len;                           /* Переменные для различных длин и количества символов */
    char sendline[1000], recvline[1000];  /* Массивы для отсылаемой и принятой строки */
    struct sockaddr_in servaddr, cliaddr; /* Структуры для адресов сервера и клиента */

    if (argc != 2)
    {
        printf("Usage: a.out <IP address>\n");
        exit(EXIT_FAILURE);
    }

    // UDP socket

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    // structer для адреса клиента

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Настройка адреса клиента
    if (bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Структура адреса сервера
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    // Настройка адреса сервера
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    {
        printf("Invalid IP address\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, listener_SIGINT);
    while (1)
    {
        printf("String => ");
        fgets(sendline, 1000, stdin);
        // sending
        if (sendto(sockfd, sendline, strlen(sendline) + 1, 0,
                   (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        // reciving
        if ((n = recvfrom(sockfd, recvline, 1000, 0, (struct sockaddr *)NULL, NULL)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("%s\n", recvline);
    }

    close(sockfd);
    return 0;
}