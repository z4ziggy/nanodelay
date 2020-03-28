#include <stdio.h>
#include <time.h>

long nanodelay(long delay_nanos)
{
    struct timespec now, start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    long nanos = 0;
    while (nanos < delay_nanos)
    {
        clock_gettime(CLOCK_MONOTONIC, &now);
        nanos = now.tv_nsec - start.tv_nsec;
        if (now.tv_sec > start.tv_sec) nanos += 1000000000L;
    }
    return nanos;
}

int main (int argc, char *argv[])
{

    struct timespec startx = {0}, endx = {0};
    long delay = 100, waited;
    sscanf(argv[1], "%lu", &delay);
    while (1) 
    {
        clock_gettime(CLOCK_MONOTONIC, &startx);
        struct timespec req={0};
        req.tv_sec=0;
        req.tv_nsec=delay ;
        //waited = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &req, NULL);
        //waited = clock_nanosleep(CLOCK_MONOTONIC, 0, &req, NULL);
        waited = nanodelay(delay);
        clock_gettime(CLOCK_MONOTONIC, &endx);
        if (endx.tv_sec - startx.tv_sec)
            endx.tv_nsec += 1000000000L;
        printf("delay %lu naonseconds took %lu nanoseconds (real %lu)\n", delay, endx.tv_nsec - startx.tv_nsec, waited);
    }

    return 0 ;
}
