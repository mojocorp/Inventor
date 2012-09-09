TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += gtest-1.6.0

win32: SUBDIRS += freetype libjpeg glu-1.3.0
mac:   SUBDIRS += freetype libjpeg
