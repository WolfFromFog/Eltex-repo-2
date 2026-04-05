#ifndef CLIENT_H
#define CLIENT_H

#define MSG_BUFF 128

extern int c_wait;
extern int client_number;

typedef struct my_msgbuf
{
    long mtype;
    char mtext[MSG_BUFF];
} my_msgbuf;

int queue_connector();
int msg_send(int msqid, char *msg, int prior);
int msg_read();
void listener_SIGINT(int sig);

#endif