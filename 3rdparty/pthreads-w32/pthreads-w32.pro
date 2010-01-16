TEMPLATE = lib
TARGET = pthreads-w32
DESTDIR  = ../../build

DEPENDPATH += .
INCLUDEPATH += .
CONFIG -= qt flat
CONFIG += static

DEFINES += PTW32_BUILD

# Input
HEADERS += config.h \
           pthread.h

SOURCES += attr.c \
           barrier.c \
           cancel.c \
           cleanup.c \
           condvar.c \
           create.c \
           dll.c \
           exit.c \
           fork.c \
           global.c \
           misc.c \
           mutex.c \
           private.c \
           rwlock.c \
           sched.c \
           semaphore.c \
           spin.c \
           sync.c \
           tsd.c \
           nonportable.c