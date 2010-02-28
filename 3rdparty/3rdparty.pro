TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = freetype libjpeg

win32: SUBDIRS += pthreads-w32
