#ifndef _SB_THREAD_MUTEX_
#define _SB_THREAD_MUTEX_

#include <Inventor/SbBasic.h>

/// Simple mutex class.
/// The mutex must be unlocked by the thread that locked it.
/// The mutex may be recursive, i.e. the same thread can lock it multiple times.
class INVENTOR_API SbThreadMutex
{
public:
    ///
    SbThreadMutex(bool recursive = false);

    ///
    ~SbThreadMutex();

    ///
    void lock();

    ///
    void unlock();

    ///
    bool trylock();
private:
    // A mutex cannot be copied
    SbThreadMutex(const SbThreadMutex&);
    SbThreadMutex& operator = (const SbThreadMutex&);

    struct SbThreadMutexP* d;
};

#endif // _SB_THREAD_MUTEX_
