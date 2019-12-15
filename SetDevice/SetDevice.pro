
QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SetDevice
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += STATIC

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG += static staticlib -static-libgcc -static-libstdc++ -libstdc++ -lstdc++

QMAKE_LFLAGS += -static

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    TableModel.cpp \
    DeviceInfo.cpp \
    sqlite3/sqlite3.c \
    iniparser/dictionary.c \
    iniparser/iniparser.c \
    iniparser/iniparsertest.cpp \
    macdialog.cpp

HEADERS += \
    mainwindow.h \
    TableModel.h \
    DeviceInfo.h \
    sqlite3/sqlite3.h \
    sqlite3/sqlite3ext.h \
    iniparser/dictionary.h \
    iniparser/iniparser.h \
    iniparser/iniparsertest.h \
    macdialog.h

FORMS += \
    mainwindow.ui \
    macdialog.ui

RESOURCES += \
    setdevice.qrc

#LIBS += -LD:/Qt/Qt5.9.6/Tools/mingw530_32/lib/gcc/i686-w64-mingw32/5.3.0 \
#        -LD:/Qt/Qt5.9.6/Tools/mingw530_32/lib/gcc/i686-w64-mingw32/lib -llibgcc_s
#        -llibstdc++

win32 {
    RC_ICONS = images/SetDevice.ico
}
