#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    char *cmdopt;
    int cmd;

    while ((cmd = getopt(argc, argv, "n:")) != -1)
        switch (cmd)
        {
        case 'n':
            cmdopt = optarg;
            break;

        case '?':
            if (optopt == 'n')
                fprintf(stderr, "", optopt); //Default error  is enough
            else if (isprint(optopt))
                fprintf(stderr, "", optopt); //Default error description is enough
            else
                fprintf(stderr, "", optopt); //Default error description is enough
            return 1;
        default:
            abort();
        }
        printf("%s", cmdopt);
        int n = atoi(cmdopt);
        printf("%d", n);

        return 0;
}