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
#include <pthread.h>

int main(int argc, char *argv[])
{
    
    struct sockaddr_in servaddr, cliaddr; /* Структуры для адресов сервера и клиента */
    pid_t pid;

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

    pid = fork();
    if (pid == 0)
    {
        recvier(sockfd);
    }
    else
    {
        char msg[] = "Registartion";
        if (sendto(sockfd, msg, strlen(msg) + 1, 0,
                   (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        sender(sockfd, &servaddr);
        kill(pid, SIGTERM);
    }
    return 0;
}