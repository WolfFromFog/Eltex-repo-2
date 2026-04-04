#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "server.c"

int main(int argc, char *argv[])
{

    int msqid = queue_creat();
    queue_delete(msqid);
}