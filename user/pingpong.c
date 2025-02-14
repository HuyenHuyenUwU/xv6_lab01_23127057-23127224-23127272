#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main()
{
    int pa[2], child[2];
    char byte = 'A';
    int buf[1] = {0};

    if (pipe(pa) < 0 || pipe(child) < 0)
    {
        printf("Pipe failed!\n");
        return 1;
    }

    int pid = fork(); // create child process

    if (pid == 0) // child process
    {
        read(pa[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(child[1], &byte, 1);

        close(pa[0]);
        close(child[1]);
        return 0;
    }
    else if (pid > 0) // parent process
    {
        write(pa[1], &byte, 1);
        read(child[0], buf, 1);
        printf("%d: received pong\n", getpid());

        close(pa[1]);
        close(child[0]);
        return 0;
    }

    printf("Fork failed!\n");
    return 1;
}