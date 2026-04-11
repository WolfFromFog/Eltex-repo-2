#ifndef PRODUCER_H
#define PRODUCER_H

#define STRING_SIZE 256

#include <stdlib.h>

extern int c_wait;

char *produce_item();
void put_item();
void listener_SIGINT(int sig);

#endif
