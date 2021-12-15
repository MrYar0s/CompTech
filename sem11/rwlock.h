#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

typedef struct Rwlock_t
{
        pthread_mutex_t mutex;

        pthread_cond_t read;
        pthread_cond_t write;

        int r_active;
        int w_active;
        int r_wait;
        int w_wait;
} rwlock_t;

int rwlock_init(rwlock_t* rwl);
int rwlock_readlock(rwlock_t* rwl);
int rwlock_writelock(rwlock_t* rwl);
int rwlock_readunlock(rwlock_t* rwl);
int rwlock_writeunlock(rwlock_t* rwl);
int rwlock_destroy(rwlock_t* rwl);

#endif//RWLOCK_H
