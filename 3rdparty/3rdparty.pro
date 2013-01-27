TEMPLATE  = subdirs
CONFIG   += ordered

win32: SUBDIRS += pthreads-w32 freetype libjpeg glu-1.3.0
mac:   SUBDIRS += freetype libjpeg
