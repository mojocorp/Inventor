#ifndef _SO_MEMORY_OBJECT_
#define _SO_MEMORY_OBJECT_

#include <Inventor/SbBasic.h>

class INVENTOR_API SoMemoryObject {
public:
    SoMemoryObject();
    ~SoMemoryObject();

    void* get() const;
    size_t length() const;

    void ref() const;
    void unref() const;
private:
    mutable int refCount;
};

#endif /* _SO_MEMORY_OBJECT_ */

