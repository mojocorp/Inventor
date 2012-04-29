TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += glew-1.7.0 gtest-1.6.0

win32: SUBDIRS += pthreads-w32 freetype libjpeg
