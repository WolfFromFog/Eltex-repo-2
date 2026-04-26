#include "networker.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    // char buff[1024];                        // Буфер для различных нужд
    int sockfd, newsockfd;                  // дескрипторы сокетов
    int portno;                             // номер порта
    int pid;                                // id номер потока
    socklen_t clilen;                       // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
    fd_set master, read_fds;                // файловые дескрипторы

    key_t semkey = ftok("server", 'S');
    struct sembuf increas = {0, 1, 0};

    nclients = semget(semkey, 1, 0666 | IPC_CREAT);
    union semun sem_union;
    sem_union.val = 0;
    semctl(nclients, 0, SETVAL, sem_union);

    printf("TCP SERVER DEMO\n");

    // ошибка в случае если мы не указали порт
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Шаг 1 - создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Шаг 2 - связывание сокета с локальным адресом
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // Шаг 3 - ожидание подключений, размер очереди - 5
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Очистка десрипторов
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(sockfd, &master);
    int FD_MAX = sockfd;
    // Семафор с
    //  Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на подключение)
    signal(SIGINT, signaler);
    while (1)
    {
        read_fds = master;
        select(FD_MAX + 1, &read_fds, NULL, NULL, NULL);

        for (int i = 0; i <= FD_MAX; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == sockfd)
                {
                    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
                    if (newsockfd < 0)
                        error("ERROR on accept");
                    if (semop(nclients, &increas, 1) == -1)
                    {
                        perror("semop: increas");
                    }
                    // вывод сведений о клиенте
                    struct hostent *hst;
                    hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
                    printf("+%s [%s] new connect!\n",
                           (hst) ? hst->h_name : "Unknown host",
                           (char *)inet_ntoa(cli_addr.sin_addr));
                    printusers();
                    FD_SET(newsockfd, &master);
                    if (newsockfd > FD_MAX)
                    {
                        FD_MAX = newsockfd;
                    }
                }
                else
                {
                    dostuff(newsockfd);
                    close(i);
                    FD_CLR(i, &master);
                    close(newsockfd);
                }
            }
        }
        /*
        pid = fork();
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0)
        {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
        */
    }
    semctl(nclients, 0, IPC_RMID);
    close(sockfd);
    return 0;
}