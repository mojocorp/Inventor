TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += glew-1.7.0 gtest-1.6.0

win32: SUBDIRS += freetype libjpeg
