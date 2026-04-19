#ifndef NETWORKER_H
#define NETWORKER_H

#include <sys/socket.h>

extern int c_wait;
extern int sockfd;

void listener_SIGINT(int sig);

void sender(int sockfd, struct sockaddr_in *servaddr);
void recvier(int sockfd);

#endif