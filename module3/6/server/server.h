#ifndef SERVER_H
#define SERVER_H

#define MSG_BUFF 128
extern int c_wait;

typedef struct my_msgbuf
{
    long mtype;
    char mtext[MSG_BUFF];
} my_msgbuf;

int queue_creat();
int msg_read();
int msg_read_test(int msqid);
int msg_send();
int queue_delete(int msqid);
void listener_SIGINT(int sig);

#endif