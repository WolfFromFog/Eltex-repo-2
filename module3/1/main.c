#include <stdio.h>
#include "checker.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{


    if (argc >= 1)
    {
        int total_argc = argc-1;
        int args_per_proc = total_argc / 2;
     
        pid_t pid;

        switch (pid = fork())
        {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
            break;
        case 0:
            printf("Дочерний процесс запущен [PID: %d]\n", getpid());
            for (int i = 1; i < args_per_proc + 1; i++)
            {
                checkNumberType(argv[i]);
            }
            printf("Дочерний процесс завершил работу.\n");
            _exit(EXIT_SUCCESS);
        default:
            printf("Родительский процесс запущен [PID: %d]\n", getpid());
            for (int i = args_per_proc + 1; i < argc; i++)
            {
                checkNumberType(argv[i]);
            }
            int status;
            wait(&status);
            printf("Дочерний процесс завреил работу с кодом: %d\n",WEXITSTATUS(status));
            printf("Родительский процесс завершил работу.\n");
            _exit(EXIT_SUCCESS);
            break;
        }
    }

    return 0;
}