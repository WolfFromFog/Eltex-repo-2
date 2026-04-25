#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include "networker.h"
#include <signal.h>

int main()
{
    // int raw_socket;
    unsigned char *buffer = (unsigned char *)malloc(65536);
    const char *dump_filename = "udp_dump.bin";

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("Ошибка создания сокета. Возможно нужен суперпользователь");
        close(sockfd);
        free(buffer);
        return 1;
    }

    printf("Ловля пакетов на порту %d...\n", TARGET_PORT);
    init_dump(dump_filename);
    signal(SIGINT, listener_SIGINT);
    while (c_wait)
    {
        int data_size = recvfrom(sockfd, buffer, 65536, 0, NULL, NULL);
        if (data_size < 0)
        {
            perror("recvfrom error");
            break;
        }
        process_packet(buffer, data_size);
    }
    close_dump();
    close(sockfd);
    free(buffer);
    return 0;
}
