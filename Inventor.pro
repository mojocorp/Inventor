######################################################################
# linux  : qmake -r -spec linux-g++
# macosx : qmake -r -spec macx-g++
# windows: qmake -r -tp vc
######################################################################
TEMPLATE  = subdirs
CONFIG   += ordered

linux-g++ {
    message(config x11)
    CONFIG += x11
}

SUBDIRS += 3rdparty libimage libInventor gtest libSoQt apps

x11: SUBDIRS += libSoXt

