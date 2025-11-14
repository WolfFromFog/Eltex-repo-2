#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    printf("Run: %s", argv[0]);
    //execl("hello"," ","Hello", "World!", NULL);
    execv("hello", argv);
    return 0;
}