#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define TARGET_PORT 51000

void process_packet(unsigned char *, int);

int main()
{
    int raw_socket;
    unsigned char *buffer = (unsigned char *)malloc(65536);

    raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_socket < 0)
    {
        perror("Socket creation failed (need root)");
        return 1;
    }

    printf("Sniffing UDP packets on port %d...\n", TARGET_PORT);

    while (1)
    {
        int data_size = recvfrom(raw_socket, buffer, 65536, 0, NULL, NULL);
        if (data_size < 0)
        {
            perror("recvfrom error");
            break;
        }
        process_packet(buffer, data_size);
    }

    close(raw_socket);
    free(buffer);
    return 0;
}

void process_packet(unsigned char *buffer, int size)
{
    struct iphdr *ip_header = (struct iphdr *)buffer;
    unsigned short ip_header_len = ip_header->ihl * 4;

    if (ip_header->protocol == 17)
    { // UDP
        struct udphdr *udp_header = (struct udphdr *)(buffer + ip_header_len);

        int src_port = ntohs(udp_header->source);
        int dst_port = ntohs(udp_header->dest);

        // Фильтрация по порту (источник или назначение)
        if (src_port == TARGET_PORT || dst_port == TARGET_PORT)
        {
            struct sockaddr_in source, dest;
            source.sin_addr.s_addr = ip_header->saddr;
            dest.sin_addr.s_addr = ip_header->daddr;

            printf("\n=== UDP Packet on port %d ===\n", TARGET_PORT);
            printf("%s:%d -> %s:%d\n",
                   inet_ntoa(source.sin_addr), src_port,
                   inet_ntoa(dest.sin_addr), dst_port);

            // Печатаем payload
            unsigned char *payload = buffer + ip_header_len + sizeof(struct udphdr);
            int payload_len = size - (ip_header_len + sizeof(struct udphdr));

            if (payload_len > 0)
            {
                printf("Data (%d bytes): ", payload_len);
                for (int i = 0; i < payload_len && i < 50; i++)
                {
                    printf("%02x ", payload[i]);
                }
                printf("\n");
            }
        }
    }
}