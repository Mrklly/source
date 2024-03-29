﻿/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtCore/QTimer>

#include "global_variables.h"

QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;
class ModbusRtu;
class Infrared;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void ClearAll();
    void about();
    void writeDataFromTextEditSend();
    void writeData(const QByteArray &data);
    void readData();
    void serialReadData();

    void handleError(QSerialPort::SerialPortError error);

    void timer_irq();
    void AutoRepeatHandler();
    void ForceSetHandler();
    void ForceSetHandler1();

    void FreshMainWindow(fresh_window_state state);

    void FreshMode();
    void FreshFan();
    void FreshTempSet();
    void FreshPowerSet();
    void ModbusSend();

    void FreshMode1();
    void FreshFan1();
    void FreshTempSet1();
    void FreshPowerSet1();
    void ModbusSend1();

    void update_uart_settings();

    void BoxUartNameChangedHandler(int index);
    void BoxBaudRateChangedHandler(int index);

    void SpinMilliSecondsChangedHandler(int milliseconds);
    void SpinAddrHandler(int number);
    void SpinAddrHandler1(int number);

    void handler_btn_mode();
    void handler_btn_fan();
    void handler_btn_up();
    void handler_btn_down();
    void handler_btn_power();

    void handler_btn_mode1();
    void handler_btn_fan1();
    void handler_btn_up1();
    void handler_btn_down1();
    void handler_btn_power1();

    void ir_send_data();
    void ir_send_cmd();
    void infrared_enter();
    void infrared_exit();

    void ir_get_data(QByteArray buf);

private:
    void initActionsConnections();
    void showStatusMessage(const QString &message);

    Ui::MainWindow *ui;
    QLabel *status;
    Console *console;
    SettingsDialog *settings;
    QSerialPort *serial;
    QTimer *timer;

    QTimer *timer_ir;

    Infrared *ir;

    ModbusRtu *rtu;
    ModbusRtu *rtu1;

    QByteArray data;
    QByteArray ir_data;

signals:

    void uart_received(QByteArray data);
    void ir_data_send_run();
    void ir_data_send_ok();

};

#endif // MAINWINDOW_H
