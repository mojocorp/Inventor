#include <Inventor/threads/SbThreadMutex.h>

#include <pthread.h>

struct SbThreadMutexP
{
    pthread_mutex_t mutex;
};

SbThreadMutex::SbThreadMutex(bool recursive)
{
    d = new SbThreadMutexP;

    if (recursive) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&d->mutex, &attr);
        pthread_mutexattr_destroy(&attr);
    } else {
        pthread_mutex_init( &d->mutex, NULL );
    }
}

SbThreadMutex::~SbThreadMutex()
{
    pthread_mutex_destroy( &d->mutex );

    delete d;
}

void SbThreadMutex::lock()
{
    pthread_mutex_lock( &d->mutex );
}

void SbThreadMutex::unlock()
{
    pthread_mutex_unlock( &d->mutex );
}

bool SbThreadMutex::trylock()
{
    return pthread_mutex_trylock( &d->mutex ) == 0;
}
