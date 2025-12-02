#include "signaler.h"
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    const char *filename = "signal_file";
    const char *str1 = "=======Начало счёта========\n";
    const char *str2 = "=======Счёт завершён=======\n";
    char buffer[32];
    log_fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
    write(log_fd, str1, strlen(str1));
    signal(SIGINT, listener_SIGINT);
    signal(SIGQUIT, listener_SIGQUIT);
    do
    {
        int len = snprintf(buffer, sizeof(buffer), "%d\n", in_counter++);
        write(log_fd, buffer, len);

        sleep(1);
    } while (c_wait);
    write(log_fd, str2, strlen(str2));
    close(log_fd);
    return 0;
}