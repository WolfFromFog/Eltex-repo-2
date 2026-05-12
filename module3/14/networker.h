#ifndef NETWORKER_H
#define NETWORKER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define TARGET_PORT 51000

extern int c_wait;
extern int sockfd;
extern FILE *dump_file;

void listener_SIGINT(int sig);

void sender(int sockfd, struct sockaddr_in *servaddr);
void recvier(int sockfd);

void process_packet(unsigned char *, int);

void init_dump(const char *filename);
void close_dump(void);

void write_to_dump(const unsigned char *data, int len);

#endif