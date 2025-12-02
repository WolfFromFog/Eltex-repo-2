#include "signaler.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int c_wait = 1;
int in_counter = 0;
int sig_counter = 0;
int log_fd = -1;

void listener_SIGINT(int sig)
{
    const char *str1 = "Получен сигнал SIGINT ||||| ";
    const char *str2 = "Обработан сигнал SIGINT.\n";
    printf("\n%s\n", "Получен сигнал SIGINT!");
    write(log_fd, str1, strlen(str1));
    sig_counter++;
    write(log_fd, str2, strlen(str2));
    if (sig_counter >= 3)
    {
        c_wait = 0;
        printf("\n%s\n", "Получено 3 сигнала SIGINT!\n");
    }
}

void listener_SIGQUIT(int sig)
{
    const char *str = "Получен и обработан сигнал SIGQUIT!";
    printf("\n%s\n", "Получен сигнал SIGQUIT!");
    write(log_fd, str, strlen(str));
}
