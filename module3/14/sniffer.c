#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include "networker.h"

void process_packet(unsigned char *, int);
void print_udp_payload(unsigned char *, int);

int main(int argc, char *argv[])
{
    // int raw_socket;
    unsigned char *buffer = (unsigned char *)malloc(65536);

    // Создание raw-сокета для UDP
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("Socket creation failed (need root)");
        return 1;
    }

    printf("UDP Sniffer started. Listening...\n");

    while (1)
    {
        int data_size = recvfrom(sockfd, buffer, 65536, 0, NULL, NULL);
        if (data_size < 0)
        {
            perror("recvfrom error");
            close(sockfd);
            return 1;
        }
        process_packet(buffer, data_size);
    }

    close(sockfd);
    free(buffer);
    return 0;
}

void process_packet(unsigned char *buffer, int size)
{
    struct iphdr *ip_header = (struct iphdr *)buffer;
    unsigned short ip_header_len = ip_header->ihl * 4;

    // Проверка, что это UDP (протокол 17)
    if (ip_header->protocol == 17)
    {
        struct udphdr *udp_header = (struct udphdr *)(buffer + ip_header_len);

        // Преобразование адресов для вывода
        struct sockaddr_in source, dest;
        source.sin_addr.s_addr = ip_header->saddr;
        dest.sin_addr.s_addr = ip_header->daddr;

        printf("\n=== UDP Packet ===\n");
        printf("Source IP: %s:%d\n", inet_ntoa(source.sin_addr), ntohs(udp_header->source));
        printf("Dest IP: %s:%d\n", inet_ntoa(dest.sin_addr), ntohs(udp_header->dest));
        printf("Length: %d\n", ntohs(udp_header->len));

        // Вывод payload (данных)
        unsigned char *payload = buffer + ip_header_len + sizeof(struct udphdr);
        int payload_len = size - (ip_header_len + sizeof(struct udphdr));

        if (payload_len > 0)
        {
            printf("Payload (%d bytes):\n", payload_len);
            print_udp_payload(payload, payload_len);
        }
    }
}

void print_udp_payload(unsigned char *payload, int len)
{
    // Печать в hex и ASCII
    for (int i = 0; i < len && i < 100; i++)
    { // лимит 100 байт для вывода
        printf("%02x ", payload[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    if (len > 100)
        printf("... (truncated)");
    printf("\n");
}