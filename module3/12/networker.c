#include "networker.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

int c_wait = 1;
int sockfd; // сокет

void listener_SIGINT(int sig)
{
    c_wait = 0;
    close(sockfd);
    _exit(EXIT_SUCCESS);
}

void sender(int sockfd, struct sockaddr_in *servaddr)
{
    char line[1000];
    while (fgets(line, 1000, stdin) != NULL)
    {
        sendto(sockfd, line, strlen(line), 0, (struct sockaddr_in *)servaddr, sizeof(servaddr));
    }
    close(sockfd);
    exit(EXIT_SUCCESS);
}

void recvier(int sockfd)
{
    char line[1000];
    int n;
    while (1)
    {
        n = recvfrom(sockfd, line, 999, 0, NULL, NULL);
        if (n > 0)
        {
            line[n] = '\0';
            printf("\r%s\n", line);
            printf("String => ");
            fflush(stdout);
        }
    }
}
