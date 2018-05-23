#ifndef _MY_PRELOAD_H
#define _MY_PRELOAD_H

#include <unistd.h>
#include <pthread.h>

#define override(name, aliasname) \
  extern __typeof (aliasname) aliasname __attribute__ ((alias (#name)));

extern pthread_key_t __libc_so_key__;

#define libc_call(_ret, _func, ...) do {			\
    void* _libc = pthread_getspecific(__libc_so_key__); \
    typeof(_func) (*_pfn) = dlsym(_libc, #_func); \
    _ret = _pfn(__VA_ARGS__); \
    } while (0)

#endif
