#pragma once

template <typename T> class SbRefPtr {
  public:
    SbRefPtr() : p(0) {}

    SbRefPtr(T *other) : p(other) {
        if (p)
            p->ref();
    }

    SbRefPtr(const SbRefPtr<T> &other) : p(other.p) {
        if (p)
            p->ref();
    }

    ~SbRefPtr() {
        if (p) {
            T *backup = p;
            p = 0;
            backup->unref();
        }
    }

    SbRefPtr<T> &operator=(const SbRefPtr<T> &other) {
        *this = other.p;
        return *this;
    }

    SbRefPtr<T> &operator=(T *other) {
        if (p != other) {
            T *backup = p;
            p = other;
            if (p)
                p->ref();
            if (backup)
                backup->unref();
        }
        return *this;
    }

       operator bool() const { return p != 0; }
    T *operator->() const { return p; }
    T &operator*() const { return *p; }
    T *get() const { return p; }

  private:
    T *p;
};

class SbRefCounted {
  public:
    void ref() { refcount++; }

    void unref() {
        if (--refcount <= 0) {
            destroy();
        }
    }

    /** should be called when it is desired to
        decrement the reference count, but not delete the instance if this
        brings the reference count to zero. This is most useful in returning
        an object to a zero-reference-count state, like it was when it was
        created.
    */
    void unrefNoDelete() { refcount--; }

    int getRefCount() const { return refcount; }

  protected:
    virtual ~SbRefCounted() {}

    virtual void destroy() { delete this; }

    SbRefCounted() : refcount(0) {}
    SbRefCounted(const SbRefCounted &) : refcount(0) {}
    SbRefCounted &operator=(const SbRefCounted &) { return *this; }

  private:
    int refcount;
};

