#include "networker.h"
#include <stdlib.h>
#include <unistd.h>

int c_wait = 1;
int sockfd;

void listener_SIGINT(int sig)
{
    c_wait = 0;
    close(sockfd);
    _exit(EXIT_SUCCESS);
}