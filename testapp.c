#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sleepms(int ms)
{
    struct timespec req;
    struct timespec rem;

    req.tv_sec = ms / 1000;
    req.tv_nsec = ms % 1000 * 1000000;
    nanosleep(&req, &rem);
}

void testcase1()
{
    printf("x");
}

void testcase2()
{
    printf("\n");
}

void testcase3()
{
    printf("\r");
}

//   500.000 | .
//   500.000 |
void testcase4()
{
    printf("\r");
}

//   500.000 | x
//     0.000 | x
void testcase5()
{
    sleepms(2500);
    printf("\n");
    sleepms(2500);
}

//   500.000 | x
//  1000.000 | x
//  1000.000 | x
//     0.000 |
void testcase6()
{
    printf("x");
    sleepms(500);
    printf("\n");
    sleepms(500);
    printf("x");
    sleepms(500);
    printf("\n");
    sleepms(500);
    printf("x");
    sleepms(500);
    printf("\n");
}

void testcase11()
{
    setvbuf(stdout, NULL, _IONBF, 0);

    time_t t;
    srand((unsigned)time(&t));

    for (int i = 0; i < 500; i++)
    {
        printf("%d", i % 10);

        sleepms(20);

        if (i % 11 == 10)
        {
            printf("\r");
        }
        if (i % 50 == 49)
        {
            printf("\n");
        }
    }

    printf("\nDone.\n");
}

void testcase12()
{
    time_t t;
    srand((unsigned)time(&t));

    for (int i = 0; i < 500; i++)
    {
        printf("%d", i % 10);

        sleepms(20);

        if (i % 11 == 10)
        {
            printf("\r");
        }
        if (i % 50 == 49)
        {
            printf("\n");
        }
    }

    printf("\nDone.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: testapp <testcasenumber>\n");
        return 1;
    }

    int testcase = atoi(argv[1]);

    switch (testcase)
    {
    case 1:
        testcase1();
        break;
    case 2:
        testcase2();
        break;
    case 3:
        testcase3();
        break;
    case 4:
        testcase4();
        break;
    case 5:
        testcase5();
        break;
    case 6:
        testcase6();
        break;
    case 11:
        testcase11();
        break;
    case 12:
        testcase12();
        break;
    default:
        printf("Unknown testcase: %d\n", testcase);
        return 1;
    }

    return 0;
}
