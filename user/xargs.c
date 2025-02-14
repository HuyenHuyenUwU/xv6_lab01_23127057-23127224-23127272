#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    int x_argc = 0;
    char *x_argv[MAXARG]; // MAXARG = 32
    int start = 0;

    if (argc > 1 && strcmp(argv[1], "-n") == 0)
        start = 2;
    else
        start = 1;

    for (int i = start; i < argc; i++)
    {
        x_argv[x_argc] = malloc(strlen(argv[i]) + 1); // +1 for '\0'
        if (!x_argv[x_argc])
        {
            printf("xargs: allocate memory failed.\n");
            exit(1);
        }
        strcpy(x_argv[x_argc++], argv[i]);
    }

    char buf;
    x_argv[x_argc] = malloc(512);
    int index = 0;

    while (read(0, &buf, 1))
    {
        if (buf == '\n')
        {
            x_argv[x_argc][index] = '\0';

            if (fork() == 0)
            {
                exec(x_argv[0], x_argv);
                printf("xargs: exec failed\n");
                exit(1);
            }
            else
            {
                wait(0);
                index = 0;
            }
        }
        else
            x_argv[x_argc][index++] = buf;
    }

    for (int j = 0; j <= x_argc; j++)
        free(x_argv[j]);

    exit(0);
}