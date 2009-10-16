TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = freetype libjpeg

win32: SUBDIRS += win-iconv pthreads-w32
