#ifndef _SB_THREAD_AUTOLOCK_
#define _SB_THREAD_AUTOLOCK_

#include <Inventor/SbBasic.h>
#include <Inventor/threads/SbThreadMutex.h>

/// Manages the mutex automatically.
/// It'll be locked when Autolock is constructed
/// and released when Autolock goes out of scope.
class INVENTOR_API SbThreadAutolock {
public:
    inline SbThreadAutolock(SbThreadMutex& mutex) : mLock(mutex)  { mLock.lock(); }
    inline SbThreadAutolock(SbThreadMutex* mutex) : mLock(*mutex) { mLock.lock(); }
    inline ~SbThreadAutolock() { mLock.unlock(); }
private:
    SbThreadMutex& mLock;
};

#endif // _SB_THREAD_AUTOLOCK_
