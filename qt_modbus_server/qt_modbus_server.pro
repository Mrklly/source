QT += core gui
QT += widgets serialport

TARGET = qt_modbus_server
TEMPLATE = app

DEFINES += STATIC

QMAKE_LFLAGS += -static

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    modbusrtu.cpp \
    infrared.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    modbusrtu.h \
    global_variables.h \
    infrared.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    infrared.ui

RESOURCES += \
    qt_modbus_server.qrc

win32 {
    RC_ICONS = modbus_icon.ico
}

#target.path = $$[QT_INSTALL_EXAMPLES]/serialport/qt_modbus_server
#INSTALLS += target

