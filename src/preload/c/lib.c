#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <pthread.h>

#ifndef LIBC_SO
#define LIBC_SO "libc.so.6"
#endif

#include "assert.h"
#include "preload.h"

int _gettimeofday(struct timeval* restrict tv, struct timezone* tz)
{
    memset(tv, 0, sizeof(struct timeval));
    return 0;
}

override(_gettimeofday, gettimeofday)

int _clock_gettime(int clock_id, struct timespec* ts)
{
    memset(ts, 0, sizeof(struct timespec));

    return 0;
}

override(_clock_gettime, clock_gettime)

#define dlfcn(_handle, _name, _alias) typeof(_alias) (* _name) = dlsym(_handle, #_alias); assert(_name)

pthread_key_t __libc_so_key__;

static void make_libc_so_key(void)
{
    (void)pthread_key_create(&__libc_so_key__, NULL);
}

void __attribute__((constructor)) lib_init(void)
{
    static pthread_once_t key_once = PTHREAD_ONCE_INIT;
    
    void* handle = dlopen("libpthread.so.0", RTLD_NOW);
    assert(handle);

    dlfcn(handle, __pthread_getspecific, pthread_getspecific);
    dlfcn(handle, __pthread_setspecific, pthread_setspecific);
    dlfcn(handle, __pthread_once, pthread_once);
    dlfcn(handle, __pthread_key_create, pthread_key_create);
    __pthread_once(&key_once, make_libc_so_key);
    if (__pthread_getspecific(__libc_so_key__) == NULL) {
	void* libc = dlopen(LIBC_SO, RTLD_NOW);
	assert(libc);

	__pthread_setspecific(__libc_so_key__, libc);
    }
}

void __attribute__((destructor)) lib_fini(void)
{
    
}

int _stat(const char* f, struct stat* buf)
{
    int r;

    libc_call(r, stat, f, buf);
    if (r < 0) return r;

    memset(&buf->st_atim, 0, sizeof(struct timespec));
    memset(&buf->st_mtim, 0, sizeof(struct timespec));
    memset(&buf->st_ctim, 0, sizeof(struct timespec));

    return r;
}

int _fstat(int fd, struct stat* buf)
{
    int r;

    libc_call(r, fstat, fd, buf);
    if (r < 0) return r;

    memset(&buf->st_atim, 0, sizeof(struct timespec));
    memset(&buf->st_mtim, 0, sizeof(struct timespec));
    memset(&buf->st_ctim, 0, sizeof(struct timespec));

    return r;
}

int _lstat(const char* f, struct stat* buf)
{
    int r;

    libc_call(r, lstat, f, buf);
    if (r < 0) return r;

    memset(&buf->st_atim, 0, sizeof(struct timespec));
    memset(&buf->st_mtim, 0, sizeof(struct timespec));
    memset(&buf->st_ctim, 0, sizeof(struct timespec));

    return r;
}

int _fstatat(int dirfd, const char* f, struct stat* buf, int flags)
{
    int r;

    libc_call(r, fstatat, dirfd, f, buf, flags);
    if (r < 0) return r;

    memset(&buf->st_atim, 0, sizeof(struct timespec));
    memset(&buf->st_mtim, 0, sizeof(struct timespec));
    memset(&buf->st_ctim, 0, sizeof(struct timespec));

    return r;

}

override(_stat, stat)
override(_fstat, fstat)
override(_lstat, lstat)
