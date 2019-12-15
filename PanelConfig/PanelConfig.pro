QT += core gui
QT += widgets serialport

TARGET = PanelConfig
TEMPLATE = app

DEFINES += STATIC

QMAKE_LFLAGS += -static

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    modbusrtu.cpp \
    settingsdialog.cpp

HEADERS += \
    mainwindow.h \
    modbusrtu.h \
    global_variables.h \
    settingsdialog.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    PanelConfig.qrc

win32 {
    RC_ICONS = PanelConfig.ico
}

#target.path = $$[QT_INSTALL_EXAMPLES]/serialport/qt_modbus_server
#INSTALLS += target

