#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#endif
#include <time.h>
#include <unistd.h>

struct timespec gettimediff(struct timespec a)
{
    struct timespec diff;
    struct timespec b;
    clock_gettime(CLOCK_MONOTONIC, &b);

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

struct timespec gettimediffresult(struct timespec a, struct timespec *b)
{
    struct timespec diff;
    clock_gettime(CLOCK_MONOTONIC, b);

    if (b->tv_nsec < a.tv_nsec)
    {
        diff.tv_sec = b->tv_sec - a.tv_sec - 1;
        diff.tv_nsec = 1000000000 + b->tv_nsec - a.tv_nsec;
    }
    else
    {
        diff.tv_sec = b->tv_sec - a.tv_sec;
        diff.tv_nsec = b->tv_nsec - a.tv_nsec;
    }

    return diff;
}

int main(int argc, char *argv[])
{
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "-NoAnsi")))
    {
        printf(
            "climon - command line monitor 0.002 gamma\n"
            "\n"
            "Usage: someapp | climon [-NoAnsi]\n"
            "\n"
            "To switch to line buffering of an app that configure itself to use full stdio buffering when piping, use this trick:\n"
            "script -q -c \"someapp somearg\" | climon\n"
            "\n"
            "Caution: The script tool will unfortunately also create a junk file named \"typescript\" in the current dir.\n");
        return 1;
    }

    int useansi = argc < 2 || strcmp(argv[1], "-NoAnsi") ? 1 : 0;

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
        if (c == '\n')
        {
            struct timespec tp;
            struct timespec diff = gettimediffresult(tpstart, &tp);

            printf("\r%5ld.%03ld\r\n%5ld.%03ld | ", diff.tv_sec, diff.tv_nsec / 1000000, 0L, 0L);
            tpstart = tp;
        }
        else if (c == '\r')
        {
            struct timespec diff = gettimediff(tpstart);
            printf("\r%5ld.%03ld | ", diff.tv_sec, diff.tv_nsec / 1000000);
        }
        else if (c < 32 && c != 9)
        {
            if (useansi)
            {
                struct timespec diff = gettimediff(tpstart);
                printf("\033[s\r%5ld.%03ld\033[u.", diff.tv_sec, diff.tv_nsec / 1000000);
            }
            else
            {
                printf(".");
            }
        }
        else
        {
            if (useansi)
            {
                struct timespec diff = gettimediff(tpstart);
                printf("\033[s\r%5ld.%03ld\033[u%c", diff.tv_sec, diff.tv_nsec / 1000000, c);
            }
            else
            {
                printf("%c", c);
            }
        }
    } while ((c = getchar()) >= 0);

    struct timespec diff = gettimediff(tpstart);

    printf("\r%5ld.%03ld\n", diff.tv_sec, diff.tv_nsec / 1000000);

    return 0;
}
