#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "networker.h"
#include <signal.h>

int main()
{
    // Для 2-ух клиентов
    struct sockaddr_in client_1, client_2;
    int client1_set = 0, client2_set = 0;
    // int sockfd;                           // Дексриптор сокета
    int clilen, n;                        // Переменные для различных длин и количества символов
    char line[1000];                      // Массив для принятой и отсылаемой строки
    struct sockaddr_in servaddr, cliaddr; // Структуры для адресов сервера и клиента

    // server addr struct
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(TARGET_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // UDP socket
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сокета
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    // Основной цикл обслуживания
    signal(SIGINT, listener_SIGINT);
    printf("Server started. Waiting for clients...\n");
    while (c_wait)
    {
        clilen = sizeof(cliaddr);
        // waiting
        if ((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr *)&cliaddr, &clilen)) < 0)
        {
            perror("recvfrom");
            close(sockfd);
            exit(1);
        }
        line[n] = '\0';
        if (strlen(line) == 0)
        {
            continue;
        }
        printf("Recived message: %s\n", line);
        int is_client1 = (client1_set && memcmp(&cliaddr, &client_1, sizeof(cliaddr)) == 0);
        int is_client2 = (client2_set && memcmp(&cliaddr, &client_2, sizeof(cliaddr)) == 0);

        if (!is_client1 && !is_client2)
        {
            if (!client1_set)
            {
                client_1 = cliaddr;
                client1_set = 1;
                printf("Client 1 registered: %s:%d\n",
                       inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                continue;
            }
            else if (!client2_set)
            {
                client_2 = cliaddr;
                client2_set = 1;
                printf("Client 2 registered: %s:%d\n",
                       inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                printf("Both registed\n");
                continue;
            }
        }

        if (is_client1 && client2_set)
        {
            sendto(sockfd, line, strlen(line), 0, (struct sockaddr *)&client_2, sizeof(client_2));
        }
        else if (is_client2 && client1_set)
        {
            sendto(sockfd, line, strlen(line), 0, (struct sockaddr *)&client_1, sizeof(client_1));
        }
    }
    return 0;
}