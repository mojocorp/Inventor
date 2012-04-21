TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += glew-1.7.0

win32: SUBDIRS += pthreads-w32 freetype libjpeg
