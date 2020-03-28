# nanodelay()
A replacement for nanosleep() procedure, for better nanoseconds resolution.

If you ever tried using [nanosleep](https://linux.die.net/man/2/clock_nanosleep) to pause your program in nanoseconds resolution, you probably read this sentence already, found in the Notes paragraph of the Man page: "If the interval specified in req is not an exact multiple of the granularity underlying clock (see time(7)), then the interval will be rounded up to the next multiple. Furthermore, after the sleep completes, there may still be a delay before the CPU becomes free to once again execute the calling thread." - yes, by design, the nanosleep will sleep extra microseconds.

For example (I am using my PC timings as an example only. Results will vary ofc on your hw):

A nanosleep with FLAGS = 0 will add extra ~60000 nanoseconds.

A nanosleep with FLAGS = TIMER_ABSTIME will add only ~3000 nanoseconds.

Thus, if you need to sleep nanoseconds without much overhead and don't feel like calculating your F_CPU mhz and clock cycles, this procedure is for you: nanodelay().
```c
void nanodelay(long delay_nanos)
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
}
```

I'm adding a small program which compares the different nanosleep methods. Here is a sample output:

## with nanosleep():
delay 1 naonseconds took 60686 nanoseconds (real 0)

delay 1 naonseconds took 60886 nanoseconds (real 0)

delay 1 naonseconds took 60882 nanoseconds (real 0)

## with naonsleep(TIMER_ABSTIME):
delay 1 naonseconds took 3705 nanoseconds (real 0)

delay 1 naonseconds took 3620 nanoseconds (real 0)

delay 1 naonseconds took 3636 nanoseconds (real 0)

## with nanodelay():
delay 1 naonseconds took 58 nanoseconds (real 18)

delay 1 naonseconds took 58 nanoseconds (real 19)

delay 1 naonseconds took 58 nanoseconds (real 18)


As you can see, delay 1 nanosecond (which is undoable on my hw but useful for testing) took minimum of 18 nanoseconds (this is the real time the procedure paused), and 58 nanoseconds, including the time it took to call the time-check procedures. 

Another solution is [changing your process priority](https://stackoverflow.com/questions/37105964/nanosleep-sleep-60-microseconds-too-long), but this doesn't always apply.
