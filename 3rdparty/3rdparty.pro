TEMPLATE  = subdirs
CONFIG   += ordered

win32: SUBDIRS += pthreads-w32 freetype libjpeg glu
mac:   SUBDIRS += freetype libjpeg
