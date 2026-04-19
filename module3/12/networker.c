#include "networker.h"
#include <stdlib.h>

int c_wait = 1;

void listener_SIGINT(int sig)
{
    c_wait = 0;
}