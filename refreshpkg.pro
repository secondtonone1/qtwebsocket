QT       += core gui
QT       += websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configOptions.cpp \
    iplistitem.cpp \
    logFile.cpp \
    main.cpp \
    msghandler.cpp \
    posthread.cpp \
    protocol.cpp \
    recvthread.cpp \
    refreshpkg.cpp \
    server.cpp \
    serverthread.cpp

HEADERS += \
    MutexMap.h \
    configOptions.h \
    constdef.h \
    devdata.h \
    iplistitem.h \
    logFile.h \
    msghandler.h \
    mutexmap.h \
    postdata.h \
    posthread.h \
    postqueue.h \
    protocol.h \
    recvdata.h \
    recvqueue.h \
    recvthread.h \
    refreshpkg.h \
    server.h \
    serverthread.h \
    singleton.h \
    uiposthread.h

FORMS += \
    refreshpkg.ui

DEFINES += QT_MESSAGELOGCONTEXT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
