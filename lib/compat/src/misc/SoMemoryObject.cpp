#include <Inventor/misc/SoMemoryObject.h>

SoMemoryObject::SoMemoryObject()
    : refCount(0)
{

}

SoMemoryObject::~SoMemoryObject()
{

}


void*
SoMemoryObject::get() const
{
    return 0;
}

size_t
SoMemoryObject::length() const
{
    return 0;
}

void
SoMemoryObject::ref() const
{
    ((SoMemoryObject *) this)->refCount++;
}

void
SoMemoryObject::unref() const
{
    if (--refCount == 0)
        delete ((SoMemoryObject *) this);
}
