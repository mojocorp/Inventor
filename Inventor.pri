INCLUDEPATH += $$PWD/libSoQt/include
INCLUDEPATH += $$PWD/libInventor/database/include \
               $$PWD/libInventor/interaction/include \
               $$PWD/libInventor/nodekits/include

LIBS += -L$$OUT_PWD/../build -lInventor
message($$OUT_PWD)
