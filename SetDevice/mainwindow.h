#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStandardItemModel>
#include <QtCore/QTimer>
#include <QScrollBar>
#include <QMessageBox>
#include <QFileDialog>

#include "sqlite3/sqlite3.h"
#include "DeviceInfo.h"
#include "macdialog.h"

namespace Ui {
class MainWindow;
}

class MACDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void window_loaded_signal();

private slots:
    void ButtonSerialRefresh_clicked();
    void ButtonSerialOpen_clicked();

    void ReadData();

    void ButtonSerialSend_clicked();
    void ButtonSerialReceiveClearClicked();

    void SetDeviceInfo();
    void TimerSerialLineWrapHandler();

    void ButtonDeviceAddHandler();
    void ButtonDeviceDeleteHandler();

    void ButtonSetbyHandHandler();
    void ButtonSetAutoHandler();

    void ActionFileNewHandler();
    void ActionFileOpenHandler();
    void ActionFileSaveHandler();
    void ActionFileExitHandler();
    void ActionFileHelpHandler();
    void ActionFileTestHandler();
    void ActionMSMACHandler();
    void BoxTypeChangedHandler(int index);

    void check_ini();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QStandardItemModel *model;
    sqlite3 *db;
    FILE *initialization;
    DeviceInfo *deviceinfo;
    QTimer *TimerSerialLineWrap;
    MACDialog *MACgenerator;

    QStringList device_type_group;

protected:
      void showEvent(QShowEvent *event);

};

#endif // MAINWINDOW_H
