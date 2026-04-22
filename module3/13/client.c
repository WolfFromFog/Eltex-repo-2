#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "networker.h"

int main(int argc, char *argv[])
{
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buff[1024];

    printf("TCP DEMO CLIENT\n");

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    // parcing port
    portno = atoi(argv[2]);
    // step 1 - creating socket
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0)
    {
        error("ERROR opening socket");
    }
    // parcing host
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(EXIT_SUCCESS);
    }

    // filling struct serv_addr
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // setting port
    serv_addr.sin_port = htons(portno);

    // step 2 - making connection
    if (connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }
    // int counter = 0;
    // step 3 - reading and sending msgs
    for (int i = 0; i < 3; i++)
    {
        n = recv(my_sock, buff, sizeof(buff) - 1, 0);
        if (n <= 0)
        {
            printf("Connection lost or erro\n");
            close(my_sock);
            return -1;
        }

        buff[n] = '\0';
        printf("S=>C:%s", buff);

        if (strstr(buff, "quit") != NULL)
        {
            close(my_sock);
            return 0;
        }

        printf("S<=C:");
        if (fgets(buff, sizeof(buff) - 1, stdin) == NULL)
        {
            printf("Input error\n");
            close(my_sock);
            return -1;
        }
        send(my_sock, buff, strlen(buff), 0);
    }
    n = recv(my_sock, buff, sizeof(buff) - 1, 0);
    if (n <= 0)
    {
        printf("Failed to reciev result\n");
        close(my_sock);
        return -1;
    }

    buff[n] = '\0';
    printf("S=>C (result): %s", buff);

    n = recv(my_sock, buff, sizeof(buff) - 1, 0);
    if (n > 0)
    {
        buff[n] = '\0';
        printf("\nS=>C: %s", buff);
    }

    close(my_sock);
    return 0;
}