#include <stdio.h>
#include <stdlib.h>
#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#endif
#include <time.h>
#include <unistd.h>

struct timespec gettimediff(struct timespec a, struct timespec b)
{
    struct timespec diff;
    if (b.tv_nsec < a.tv_nsec)
    {
        diff.tv_sec = b.tv_sec - a.tv_sec - 1;
        diff.tv_nsec = 1000000000 + b.tv_nsec - a.tv_nsec;
    }
    else
    {
        diff.tv_sec = b.tv_sec - a.tv_sec;
        diff.tv_nsec = b.tv_nsec - a.tv_nsec;
    }

    return diff;
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf(
            "climon - command line monitor 0.001 gamma\n"
            "\n"
            "Usage: someapp | climon\n"
            "\n"
            "To switch to line buffering of an app that configure itself to use full stdio buffering when piping, use this trick:\n"
            "script -q -c \"someapp somearg\" | climon\n"
            "\n"
            "Caution: The script tool will unfortunately also create a junk file named \"typescript\" in the current dir.\n");
        return 1;
    }

    if (isatty(fileno(stdin)))
    {
        printf("Only piped input supported.\n");
        return 1;
    }

    struct timespec tpstart;
    clock_gettime(CLOCK_MONOTONIC, &tpstart);

    int c = getchar();
    if (c < 0)
    {
        printf("No input.\n");
        return 1;
    }

    setvbuf(stdout, NULL, _IONBF, 0);

    printf("%5ld.%03ld | ", 0L, 0L);

    do
    {
        struct timespec tp;
        clock_gettime(CLOCK_MONOTONIC, &tp);

        struct timespec diff = gettimediff(tpstart, tp);

        if (c == '\n')
        {
            printf("\r%5ld.%03ld\r\n", diff.tv_sec, diff.tv_nsec / 1000000);
            tpstart = tp;

            printf("%5ld.%03ld | ", 0L, 0L);
        }
        else if (c == '\r')
        {
            printf("\r%5ld.%03ld | ", diff.tv_sec, diff.tv_nsec / 1000000);
        }
        else if (c < 32 && c != 9)
        {
            printf("\033[s\r%5ld.%03ld\033[u.", diff.tv_sec, diff.tv_nsec / 1000000);
        }
        else
        {
            printf("\033[s\r%5ld.%03ld\033[u%c", diff.tv_sec, diff.tv_nsec / 1000000, c);
        }
    } while ((c = getchar()) >= 0);

    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    struct timespec diff = gettimediff(tpstart, tp);

    printf("\033[s\r%5ld.%03ld\n", diff.tv_sec, diff.tv_nsec / 1000000);

    return 0;
}
