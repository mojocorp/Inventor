TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS =  libjpeg

win32: SUBDIRS += pthreads-w32 freetype
