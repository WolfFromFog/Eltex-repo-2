#include "networker.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <time.h>

int c_wait = 1;
int sockfd = -1; // сокет
FILE *dump_file = NULL;

void listener_SIGINT(int sig)
{
    c_wait = 0;
    if (sockfd > 0)
    {
        close(sockfd);
        sockfd = -1;
    }
}

void sender(int sockfd, struct sockaddr_in *servaddr)
{
    char line[1000];
    while (c_wait)
    {
        if (fgets(line, 1000, stdin) != NULL)
        {
            sendto(sockfd, line, strlen(line), 0, (struct sockaddr *)servaddr, sizeof(*servaddr));
        }
        else
        {
            break;
        }
    }
    // close(sockfd);
    // exit(EXIT_SUCCESS);
}

void recvier(int sockfd)
{
    char line[1000];
    int n;
    while (c_wait)
    {
        n = recvfrom(sockfd, line, 999, 0, NULL, NULL);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            break;
        }
        if (n > 0)
        {
            line[n] = '\0';
            printf("\r%s\n", line);
            printf("=> ");
            fflush(stdout);
        }
    }
    // close(sockfd);
    // exit(EXIT_SUCCESS);
}

void process_packet(unsigned char *buffer, int size)
{
    struct iphdr *ip_header = (struct iphdr *)buffer;
    unsigned short ip_header_len = ip_header->ihl * 4;

    struct udphdr *udp_header = (struct udphdr *)(buffer + ip_header_len);

    int src_port = ntohs(udp_header->source);
    int dst_port = ntohs(udp_header->dest);

    // Фильтрация по порту
    if (dst_port == TARGET_PORT)
    {
        struct sockaddr_in source, dest;
        source.sin_addr.s_addr = ip_header->saddr;
        dest.sin_addr.s_addr = ip_header->daddr;

        printf("\n%s:%d -> %s:%d\n",
               inet_ntoa(source.sin_addr), src_port,
               inet_ntoa(dest.sin_addr), dst_port);

        // Печатаем payload
        unsigned char *payload = buffer + ip_header_len + sizeof(struct udphdr);
        int payload_len = size - (ip_header_len + sizeof(struct udphdr));

        if (payload_len > 0)
        {
            printf("Данные (%d байт): ", payload_len);
            for (int i = 0; i < payload_len && i < 50; i++)
            {
                if (payload[i] >= 32 && payload[i] <= 126)
                {
                    printf("%c", payload[i]);
                }
                else
                {
                    printf("%02x ", payload[i]);
                }
            }
            printf("\n");
            write_to_dump(payload, payload_len);
        }
    }
}

void init_dump(const char *filename)
{
    dump_file = fopen(filename, "wb");
    if (dump_file == NULL)
    {
        perror("Не удалось создать дампа");
    }
    else
    {

        fflush(dump_file);
    }
}

void close_dump(void)
{
    if (dump_file != NULL)
    {
        fclose(dump_file);
        dump_file = NULL;
        printf("Файл дампа закрыт\n");
    }
}

void write_to_dump(const unsigned char *data, int len)
{
    if (dump_file == NULL || len <= 0)
        return;
        
    fwrite(data, 1, len, dump_file);
    fflush(dump_file);
}
