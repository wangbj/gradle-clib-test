// #include <sys/time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#ifndef LIBC_SO
#define LIBC_SO "libc.so.6"
#endif

struct timespec;
struct timezone;

int gettimeofday(struct timespec* tv, struct timezone* tz)
{
    memset(tv, 0, 2*sizeof(long));
    return 0;
}

