#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "networker.h"
#include <signal.h>

int main()
{
    int sockfd;                           // Дексриптор сокета
    int clilen, n;                        // Переменные для различных длин и количества символов
    char line[1000];                      // Массив для принятой и отсылаемой строки
    struct sockaddr_in servaddr, cliaddr; // Структуры для адресов сервера и клиента

    // server addr struct
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // UDP socket
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сокета
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    // Основной цикл обслуживания
    signal(SIGINT, listener_SIGINT);
    while (c_wait)
    {
        clilen = sizeof(cliaddr);
        // waiting
        if ((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr *)&cliaddr, &clilen)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        if (strlen(line) > 0)
        {
            printf("%s\n", line);

            if (sendto(sockfd, line, strlen(line), 0,
                       (struct sockaddr *)&cliaddr, clilen) < 0)
            {
                perror(NULL);
                close(sockfd);
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}