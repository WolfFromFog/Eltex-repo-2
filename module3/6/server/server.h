#ifndef SERVER_H
#define SERVER_H

#define MSG_BUFF 128
extern int c_wait;

extern long int *clients;

typedef struct my_msgbuf
{
    long mtype;
    char mtext[MSG_BUFF];
} my_msgbuf;

// Очередь
int queue_creat();
int queue_delete(int msqid);

// Сообщения
int msg_read(int msqid);
int msg_read_test(int msqid);
int msg_send(int msqid, char *msg2send, long int mtype);

// Вспомогательные
void listener_SIGINT(int sig);
void addClient2Array(long int **array, int *size, long int id);
void removeClientfromArray(long int **array, int *size, long int id);
#endif