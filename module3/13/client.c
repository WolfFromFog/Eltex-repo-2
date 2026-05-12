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
    int use_file_mode = 0;

    printf("TCP CLIENT\n");
    printf("Available modes:\n");
    printf("  1. Interactive mode (default)\n");
    printf("  2. File mode - type 'file' when prompted\n\n");

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

        if (strstr(buff, "FILE_MODE_READY") != NULL)
        {
            use_file_mode = 1;
            break;
        }

        printf("S<=C:");
        if (fgets(buff, sizeof(buff) - 1, stdin) == NULL)
        {
            printf("Input error\n");
            close(my_sock);
            return -1;
        }

        buff[strcspn(buff, "\n")] = 0;

        send(my_sock, buff, strlen(buff), 0);

        if (strcmp(buff, "file") == 0)
        {
            // Ждем подтверждение перехода в файловый режим
            n = recv(my_sock, buff, sizeof(buff) - 1, 0);
            if (n > 0)
            {
                buff[n] = '\0';
                printf("S=>C:%s", buff);
                use_file_mode = 1;
                break;
            }
        }
    }

    if (use_file_mode)
    {
        char filename[1024];
        printf("Enter file name to send: ");

        if (fgets(filename, sizeof(filename), stdin) == NULL)
        {
            printf("Error reading filename\n");
            close(my_sock);
            return -1;
        }
        filename[strcspn(filename, "\n")] = 0;

        if (access(filename, F_OK) != 0)
        {
            printf("Error: File '%s' does not exist\n", filename);
            close(my_sock);
            return -1;
        }

        printf("Sending file: %s\n", filename);
        if (send_file(my_sock, filename) < 0)
        {
            printf("Failed to send file to server\n");
            close(my_sock);
            return -1;
        }

        printf("Waiting for server response...\n");
        char response_file[] = "server_response.txt";
        if (receive_file(my_sock, response_file) < 0)
        {
            printf("Failed to receive response file\n");
            close(my_sock);
            return -1;
        }

        FILE *result_file = fopen(response_file, "r");
        if (result_file != NULL)
        {
            char line[256];
            printf("\n=== RESULT ===\n");
            while (fgets(line, sizeof(line), result_file))
            {
                printf("%s", line);
            }
            fclose(result_file);
        }

        n = recv(my_sock, buff, sizeof(buff) - 1, 0);
        if (n > 0)
        {
            buff[n] = '\0';
            printf("\nS=>C: %s", buff);
        }
    }
    else
    {
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
    }

    close(my_sock);
    return 0;
}