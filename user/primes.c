#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

void primes(int leftPipe)
{
    int prime;
    if (read(leftPipe, &prime, sizeof(int)) != sizeof(int))
    {
        close(leftPipe);
        return;
    }

    printf("primes %d\n", prime);

    int rightPipe[2];
    if (pipe(rightPipe) < 0)
    {
        printf("pipe failed");
        return;
    }

    int pid = fork();

    if (pid < 0)
    {
        printf("fork failed");
        close(rightPipe[1]);
        close(rightPipe[0]);
        close(leftPipe);
        return;
    }
    if (pid == 0) // child process
    {
        close(rightPipe[1]);
        close(leftPipe);
        primes(rightPipe[0]);
        close(rightPipe[0]);
    }
    else // parent process
    {
        close(rightPipe[0]);
        int num;
        while (read(leftPipe, &num, sizeof(int)) > 0)
            if (num % prime != 0)
                write(rightPipe[1], &num, sizeof(int));

        close(rightPipe[1]);
        close(leftPipe);
        wait(0);
    }

    close(rightPipe[1]);
    close(rightPipe[0]);
    return;
}

int main()
{
    int p[2];
    if (pipe(p) < 0)
    {
        printf("pipe failed");
        return 1;
    }

    int pid = fork();

    if (pid < 0)
    {
        printf("fork failed");
        return 1;
    }

    if (pid == 0) // child process
    {
        close(p[1]);
        primes(p[0]);
    }
    else // parent process
    {
        close(p[0]);
        for (int i = 2; i < 281; i++)
            write(p[1], &i, sizeof(int));

        close(p[1]);
        wait(0);
    }

    return 0;
}
