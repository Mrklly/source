/****************************************************************************
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"
#include "modbusrtu.h"
#include "infrared.h"

#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QColorDialog>
#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);
//  setCentralWidget(console);

    serial = new QSerialPort(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_irq()));

    rtu = new ModbusRtu;
    rtu1 = new ModbusRtu;

    rtu->data.rtu_address = 0x03;
    rtu1->data.rtu_address = 0x04;

    rtu->g_fan_ultralow = 0;
    rtu1->g_fan_ultralow = 0;

    settings = new SettingsDialog;

    ir = new Infrared;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

//    ui->LcdRoomTemp->setPalette(Qt::red);

    ui->CheckAutoRepeat->setChecked(false);

    ui->CheckForceSet->setChecked(false);
    ui->CheckForceSet1->setCheckable(false);

    status = new QLabel;
    ui->statusBar->addWidget(status);

    ui->ButtonValveStatus->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::red).dark(140).name()));    // background: #aa00ff;
    ui->ButtonValveStatus->update();

    ui->ButtonValveStatus1->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::red).dark(140).name()));    // background: #aa00ff;
    ui->ButtonValveStatus1->update();

    ui->BoxUartName->addItems(settings->GetSerialPortNameAll());
    ui->BoxBaudRate->setCurrentText(settings->GetSerialPortBaud());

    ui->LabelFanAuto->setStyleSheet("QLabel { color : red; }");   // setStyleSheet("QLabel { background-color : white; color : red; }")
    ui->LabelFanAuto1->setStyleSheet("QLabel { color : red; }");
    ui->LabelFanAuto->hide();
    ui->LabelFanAuto1->hide();

    initActionsConnections();

    connect(settings, SIGNAL(settingdialog_update()), this, SLOT(update_uart_settings()));

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);


    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    connect(ui->ButtonSend, &QPushButton::clicked, this, &MainWindow::writeDataFromTextEditSend);

    connect(ui->CheckAutoRepeat, &QCheckBox::stateChanged, this, AutoRepeatHandler);

    connect(ui->CheckForceSet, &QCheckBox::stateChanged, this, ForceSetHandler);
    connect(ui->CheckForceSet1, &QCheckBox::stateChanged, this, ForceSetHandler1);

    connect(ui->BoxUartName, SIGNAL(currentIndexChanged(int)), SLOT(BoxUartNameChangedHandler(int)));
    connect(ui->BoxBaudRate, SIGNAL(currentIndexChanged(int)), SLOT(BoxBaudRateChangedHandler(int)));

    ui->SpinMilliSeconds->setKeyboardTracking(false);
    connect(ui->SpinMilliSeconds, SIGNAL(valueChanged(int)), SLOT(SpinMilliSecondsChangedHandler(int)));

    ui->SpinDevAddr->setKeyboardTracking(false);
    ui->SpinDevAddr1->setKeyboardTracking(false);
    connect(ui->SpinDevAddr,  SIGNAL(valueChanged(int)), this, SLOT(SpinAddrHandler(int)));
    connect(ui->SpinDevAddr1, SIGNAL(valueChanged(int)), this, SLOT(SpinAddrHandler1(int)));

    QObject::connect(ui->ButtonMode, SIGNAL(clicked()), this, SLOT(handler_btn_mode()));
    QObject::connect(ui->ButtonFan,  SIGNAL(clicked()), this, SLOT(handler_btn_fan()));
    QObject::connect(ui->ButtonUp,   SIGNAL(clicked()), this, SLOT(handler_btn_up()));
    QObject::connect(ui->ButtonDown, SIGNAL(clicked()), this, SLOT(handler_btn_down()));
    QObject::connect(ui->ButtonPower,SIGNAL(clicked()), this, SLOT(handler_btn_power()));
// rtu1
    QObject::connect(ui->ButtonMode1, SIGNAL(clicked()), this, SLOT(handler_btn_mode1()));
    QObject::connect(ui->ButtonFan1,  SIGNAL(clicked()), this, SLOT(handler_btn_fan1()));
    QObject::connect(ui->ButtonUp1,   SIGNAL(clicked()), this, SLOT(handler_btn_up1()));
    QObject::connect(ui->ButtonDown1, SIGNAL(clicked()), this, SLOT(handler_btn_down1()));
    QObject::connect(ui->ButtonPower1,SIGNAL(clicked()), this, SLOT(handler_btn_power1()));

    QObject::connect(this, &MainWindow::uart_received, ir, &Infrared::DataToFile);
    QObject::connect(ir, &Infrared::DataToSend, this, &MainWindow::ir_get_data);
    QObject::connect(this, &MainWindow::ir_data_send_run, this, &MainWindow::ir_send_data);
    QObject::connect(ir, &Infrared::InfraredEnter, this, &MainWindow::infrared_enter);
    QObject::connect(ir, &Infrared::InfraredExit, this, &MainWindow::infrared_exit);
    QObject::connect(this, &MainWindow::ir_data_send_ok, ir, &Infrared::DataSendOk);

}


MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}


void MainWindow::openSerialPort()
{
    if (serial->isOpen()) {
        return ;
    }

    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

        ui->CheckAutoRepeat->setCheckable(true);
        if(ir->isActiveWindow() == false) {
            ui->CheckAutoRepeat->setChecked(true);
        } else {
            ui->CheckAutoRepeat->setChecked(false);
        }

        ui->CheckForceSet->setChecked(true);
        ui->CheckForceSet1->setChecked(true);
        ui->CheckDevEnabled->setChecked(true);
        ui->CheckDevEnabled1->setChecked(true);

        ui->CheckForceSet->setCheckable(true);
        ui->CheckForceSet1->setCheckable(true);
        ui->CheckDevEnabled->setCheckable(true);
        ui->CheckDevEnabled1->setCheckable(true);
        ui->CheckDevEnabled1->setChecked(true);
        AutoRepeatHandler();

    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        ui->CheckAutoRepeat->setChecked(false);
        ui->CheckAutoRepeat->setCheckable(false);
        ui->CheckForceSet->setChecked(false);
        ui->CheckForceSet->setCheckable(false);
        ui->CheckForceSet1->setChecked(false);
        ui->CheckForceSet1->setCheckable(false);
        ui->CheckDevEnabled->setChecked(false);
        ui->CheckDevEnabled1->setChecked(false);
        ui->CheckDevEnabled->setCheckable(false);
        ui->CheckDevEnabled1->setCheckable(false);
        showStatusMessage(tr("Open error"));
    }
}



void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    timer->stop();
    ui->CheckAutoRepeat->setChecked(false);
    ui->CheckAutoRepeat->setCheckable(false);
    ui->CheckForceSet->setChecked(false);
    ui->CheckForceSet->setCheckable(false);
    ui->CheckForceSet1->setChecked(false);
    ui->CheckForceSet1->setCheckable(false);
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::ClearAll()
{
    ui->TextReceive->clear();
    ui->TextSend->clear();
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("关于温控测试上位机\n"),
                       tr("<b>温控测试上位机</b> 是对温控面板进行测试的电脑端软件，\n"
                          "操作简单，界面直观，稳定可靠"));
}


void MainWindow::writeDataFromTextEditSend()
{
    QString str = ui->TextSend->toPlainText();
    QByteArray data = QByteArray::fromHex( str.toLatin1() );
//    QByteArray data_array = ui->TextSend->toPlainText().toLocal8Bit();


    serial->write(data);
//    QStringList data_line = data_str.split(QRegExp("[\n]"),QString::SkipEmptyParts);

}


void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}



void MainWindow::readData()
{
    unsigned int crc16;

    if(ir->ir_func == IR_FUNC_RECV) {
        emit uart_received(serial->readAll());
        return ;
    } else if(ir->ir_func == IR_FUNC_SEND) {

        data.append(serial->readAll());

        if(data.length() >= 4) {

            if(data[0] == 0x55 && data[1] == 0xaa && data[2] == ir->addr && data[3] == 0x5a) {
                emit ir_data_send_run();
                data.clear();
            }
        }

        return ;
    }

    data.append(serial->readAll());

    if(data.length() >= 6) {
        rtu-> data.register_number = data[5];
        rtu1->data.register_number = data[5];
    }

    if(data.length() >= 4) {
        if(data[3] != CTRL_FUN_RAED && data[3] != CTRL_FUN_WRITE) {
            data.clear();
            return ;
        }
    }

    if(data.length() >= 3) {

    }

    if(data.length() >= 2) {
        if((data[1] != rtu->data.sync_head2) || (data[1] != rtu1->data.sync_head2)) {
            data.clear();
            return ;
        }
    }

    if(data.length() >= 1) {
        if((data[0] != rtu->data.sync_head1) || ((data[0] != rtu1->data.sync_head1))) {
            data.clear();
            return ;
        }
    }

    if((data.length() < 6 + rtu->data.register_number + 2) || (data.length() < 6 + rtu1->data.register_number + 2)) {
        return ;
    }

    rtu->data.crc_high = data[rtu->data.register_number + 6];
    rtu->data.crc_low  = data[rtu->data.register_number + 7];

    QByteArray crc_data = data.mid(2, rtu->data.register_number + 4);
    crc16 = rtu->modbus_crc16((unsigned char *)crc_data.data(), rtu->data.register_number + 4); // calculate CRC
    if(( (unsigned char)(crc16&0xff) != rtu->data.crc_high ) ||
            ( (unsigned char)(crc16>>8 )  != rtu->data.crc_low )) {
        data.clear();
        return ;
    }

    if(data[3] == 0x10) {              // when send commands to temperature controler, it returns the same data
        char temp_len = data.length(); // only show the returned value
        QString temp_string = data.toHex().toUpper();
        for(int i = 1; i < temp_len; i++){
            temp_string.insert(3*i-1,' ');
        }
        ui->TextReceive->append(temp_string);
        data.clear();
        return ;
    }

    if(data[2] == rtu->data.rtu_address) {
        rtu->data.data = data.mid(6, rtu->data.register_number);
        rtu->set_temp = rtu->data.data[1];
        rtu->device_status = rtu->data.data[2];
        rtu->room_temp = (float)((uint16_t)(rtu->data.data[3]) << 8 | ((uint16_t)(rtu->data.data[4]))&0xff) / 10;

        uint8_t dev_sta_wind = 0;
        QString dev_sta_mod, dev_sta_power, dev_sta_valve;

        switch (rtu->mode.mod) {
        case 0:
            dev_sta_mod = "制冷 Cool";
            break;
        case 1:
            dev_sta_mod = "制热 Warm";
            break;
        case 2:
            dev_sta_mod = "送风 Fan";
            break;
        case 3:

            break;
        case 4:

            break;
        default:
            break;
        }

        if(rtu->mode.power) {
            dev_sta_power = "已开机 ON";
            ui->LabelPower->setStyleSheet("color: green");
            ui->ButtonPower->setStyleSheet("color: red");  // color:rgb(255, 0, 0)
        } else {
            dev_sta_power = "已关机 OFF";
            ui->LabelPower->setStyleSheet("color: red");
            ui->ButtonPower->setStyleSheet("color: green");  // color:rgb(255, 0, 0)
        }

        if(rtu->mode.valve) {
            dev_sta_valve = "阀门已打开";
            ui->ButtonValveStatus->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::green).dark(140).name()));    // background: #aa00ff;
        } else {
            dev_sta_valve = "阀门已关闭";
            ui->ButtonValveStatus->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::red).dark(140).name()));    // background: #aa00ff;
    //                ui->ButtonValveStatus->update();
        }

        if(rtu->set_temp & 0x80) {
            rtu->g_fan_ultralow = 1;
            rtu->set_temp &= ~0x80;
        } else {
            rtu->g_fan_ultralow = 0;
        }

        FreshFan();

        ui->LcdSetTemp->display(rtu->set_temp);
        ui->LcdRoomTemp->display(rtu->room_temp);
        ui->LabelShowMode->setText(dev_sta_mod);
        ui->LabelPower->setText(dev_sta_power);
        ui->ButtonValveStatus->setText(dev_sta_valve);
    } else if(data[2] == rtu1->data.rtu_address) {                    // device 2
        rtu1->data.data = data.mid(6, rtu1->data.register_number);
        rtu1->set_temp = rtu1->data.data[1];
        rtu1->device_status = rtu1->data.data[2];
        rtu1->room_temp = (float)((uint16_t)(rtu1->data.data[3]) << 8 | ((uint16_t)(rtu1->data.data[4]))&0xff) / 10;

        uint8_t dev_sta_wind = 0;
        QString dev_sta_mod, dev_sta_power, dev_sta_valve;

        switch (rtu1->mode.mod) {
        case 0:
            dev_sta_mod = "制冷 Cool";
            break;
        case 1:
            dev_sta_mod = "制热 Warm";
            break;
        case 2:
            dev_sta_mod = "送风 Fan";
            break;
        case 3:

            break;
        case 4:

            break;
        default:
            break;
        }

        if(rtu1->mode.power) {
            dev_sta_power = "已开机 ON";
            ui->LabelPower1->setStyleSheet("color: green");
            ui->ButtonPower1->setStyleSheet("color: red");  // color:rgb(255, 0, 0)
        } else if(rtu1->mode.power == 0){
            dev_sta_power = "已关机 OFF";
            ui->LabelPower1->setStyleSheet("color: red");
            ui->ButtonPower1->setStyleSheet("color: green");  // color:rgb(255, 0, 0)
        }

        if(rtu1->mode.valve) {
            dev_sta_valve = "阀门已打开";
            ui->ButtonValveStatus1->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::green).dark(140).name()));    // background: #aa00ff;
        } else if(rtu1->mode.valve == 0) {
            dev_sta_valve = "阀门已关闭";
            ui->ButtonValveStatus1->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::red).dark(140).name()));    // background: #aa00ff;
    //                ui->ButtonValveStatus->update();
        }

        if(rtu1->set_temp & 0x80) {
            rtu1->g_fan_ultralow = 1;
            rtu1->set_temp &= ~0x80;
        } else {
            rtu1->g_fan_ultralow = 0;
        }

        FreshFan1();

        ui->LcdSetTemp1->display(rtu1->set_temp);
        ui->LcdRoomTemp1->display(rtu1->room_temp);

        ui->LabelShowMode1->setText(dev_sta_mod);
        ui->LabelPower1->setText(dev_sta_power);
        ui->ButtonValveStatus1->setText(dev_sta_valve);
    }

    rtu->data.data.clear();
    rtu1->data.data.clear();
    char temp_len = data.length();
    QString temp_string = data.toHex().toUpper();
    for(int i = 1; i < temp_len; i++){
        temp_string.insert(3*i-1,' ');
    }

    ui->TextReceive->append(temp_string);
    data.clear();

#if 0
    if(data.length() == 1) {
    } else if(data.length() == 2) {
    } else if(data.length() >= 6) {
        if((data[0] != rtu->data.sync_head1) || (data[1] != rtu->data.sync_head2)) {
            data.clear();
            return ;
        }

        rtu1->data.sync_head1 = rtu->data.sync_head1;
        rtu1->data.sync_head2 = rtu->data.sync_head2;

        if(data[2] == 0x03) {
            rtu->data.register_number = data[5];
            rtu->data.crc_high = data[rtu->data.register_number + 6];
            rtu->data.crc_low = data[rtu->data.register_number + 7];

            if(data.length() < 6 + rtu->data.register_number + 2) {
                return ;
            }

            QByteArray crc_data = data.mid(2, rtu->data.register_number + 4);
            crc16 = rtu->modbus_crc16((unsigned char *)crc_data.data(), rtu->data.register_number + 4); // calculate CRC
            if(( (unsigned char)(crc16&0xff) != rtu->data.crc_high ) ||
                    ( (unsigned char)(crc16>>8 )  != rtu->data.crc_low )) {
                data.clear();
                return ;
            }

            if(data[3] == 0x10) {              // when send commands to temperature controler, it returns the same data
                char temp_len = data.length(); // only show the returned value
                QString temp_string = data.toHex().toUpper();
                for(int i = 1; i < temp_len; i++){
                    temp_string.insert(3*i-1,' ');
                }
                ui->TextReceive->append(temp_string);
                data.clear();
                return ;
            }

            rtu->data.data = data.mid(6,rtu->data.register_number);
            rtu->set_temp = rtu->data.data[1];
            rtu->device_status = rtu->data.data[2];
            rtu->room_temp = (float)((uint16_t)(rtu->data.data[3]) << 8 | ((uint16_t)(rtu->data.data[4]))&0xff) / 10;

            uint8_t dev_sta_wind = 0;
            QString dev_sta_mod, dev_sta_power, dev_sta_valve;

            switch (rtu->mode.wind) {
            case 0:
                dev_sta_wind = 0;
                break;
            case 1:
                dev_sta_wind = 25;
                break;
            case 2:
                dev_sta_wind = 50;
                break;
            case 3:
                dev_sta_wind = 100;
                break;
            default:
                break;
            }

            switch (rtu->mode.mod) {
            case 0:
                dev_sta_mod = "制冷 Cool";
                break;
            case 1:
                dev_sta_mod = "制热 Warm";
                break;
            case 2:
                dev_sta_mod = "送风 Fan";
                break;
            case 3:

                break;
            case 4:

                break;
            default:
                break;
            }

            if(rtu->mode.power) {
                dev_sta_power = "已开机 ON";
                ui->LabelPower->setStyleSheet("color: green");
                ui->ButtonPower->setStyleSheet("color: red");  // color:rgb(255, 0, 0)
            } else {
                dev_sta_power = "已关机 OFF";
                ui->LabelPower->setStyleSheet("color: red");
                ui->ButtonPower->setStyleSheet("color: green");  // color:rgb(255, 0, 0)
            }

            if(rtu->mode.valve) {
                dev_sta_valve = "阀门已打开";
                ui->ButtonValveStatus->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::green).dark(140).name()));    // background: #aa00ff;
            } else {
                dev_sta_valve = "阀门已关闭";
                ui->ButtonValveStatus->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::red).dark(140).name()));    // background: #aa00ff;
    //                ui->ButtonValveStatus->update();
            }

            ui->LcdRoomTemp->display(rtu->room_temp);
            ui->LcdSetTemp->display(rtu->set_temp);
            ui->BarFanState->setValue(dev_sta_wind);
            ui->LabelShowMode->setText(dev_sta_mod);
            ui->LabelPower->setText(dev_sta_power);
            ui->ButtonValveStatus->setText(dev_sta_valve);

            rtu->data.data.clear();
            char temp_len = data.length();
            QString temp_string = data.toHex().toUpper();
            for(int i = 1; i < temp_len; i++){
                temp_string.insert(3*i-1,' ');
            }
            ui->TextReceive->append(temp_string);
            data.clear();

        } else if(data[2] == 0x04) {
            rtu1->data.register_number = data[5];
            rtu1->data.crc_high = data[rtu1->data.register_number + 6];
            rtu1->data.crc_low = data[rtu1->data.register_number + 7];

            if(data.length() < 6 + rtu1->data.register_number + 2) {
                return ;
            }

            QByteArray crc_data = data.mid(2, rtu1->data.register_number + 4);
            crc16 = rtu1->modbus_crc16((unsigned char *)crc_data.data(), rtu1->data.register_number + 4); // calculate CRC
            if(( (unsigned char)(crc16&0xff) != rtu1->data.crc_high ) ||
                    ( (unsigned char)(crc16>>8 )  != rtu1->data.crc_low )) {
                data.clear();
                return ;
            }

            if(data[3] == 0x10) {              // when send commands to temperature controler, it returns the same data
                char temp_len = data.length(); // only show the returned value
                QString temp_string = data.toHex().toUpper();
                for(int i = 1; i < temp_len; i++){
                    temp_string.insert(3*i-1,' ');
                }
                ui->TextReceive->append(temp_string);
                data.clear();
                return ;
            }

            rtu1->data.data = data.mid(6,rtu1->data.register_number);
            rtu1->set_temp = rtu1->data.data[1];
            rtu1->device_status = rtu1->data.data[2];
            rtu1->room_temp = (float)((uint16_t)(rtu1->data.data[3]) << 8 | ((uint16_t)(rtu1->data.data[4]))&0xff) / 10;

            uint8_t dev_sta_wind = 0;
            QString dev_sta_mod, dev_sta_power, dev_sta_valve;

            switch (rtu1->mode.wind) {
            case 0:
                dev_sta_wind = 0;
                break;
            case 1:
                dev_sta_wind = 25;
                break;
            case 2:
                dev_sta_wind = 50;
                break;
            case 3:
                dev_sta_wind = 100;
                break;
            default:
                break;
            }

            switch (rtu1->mode.mod) {
            case 0:
                dev_sta_mod = "制冷 Cool";
                break;
            case 1:
                dev_sta_mod = "制热 Warm";
                break;
            case 2:
                dev_sta_mod = "送风 Fan";
                break;
            case 3:

                break;
            case 4:

                break;
            default:
                break;
            }

            if(rtu1->mode.power) {
                dev_sta_power = "已开机 ON";
                ui->LabelPower1->setStyleSheet("color: green");
                ui->ButtonPower1->setStyleSheet("color: red");  // color:rgb(255, 0, 0)
            } else {
                dev_sta_power = "已关机 OFF";
                ui->LabelPower1->setStyleSheet("color: red");
                ui->ButtonPower1->setStyleSheet("color: green");  // color:rgb(255, 0, 0)
            }

            if(rtu1->mode.valve) {
                dev_sta_valve = "阀门已打开";
                ui->ButtonValveStatus1->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::green).dark(140).name()));    // background: #aa00ff;
            } else {
                dev_sta_valve = "阀门已关闭";
                ui->ButtonValveStatus1->setStyleSheet(QString("background-color: %1; color: rgb(255, 255, 255); border: none;").arg(QColor(Qt::red).dark(140).name()));    // background: #aa00ff;
    //                ui->ButtonValveStatus->update();
            }

            ui->LcdRoomTemp1->display(rtu1->room_temp);
            ui->LcdSetTemp1->display(rtu1->set_temp);
            ui->BarFanState1->setValue(dev_sta_wind);
            ui->LabelShowMode1->setText(dev_sta_mod);
            ui->LabelPower1->setText(dev_sta_power);
            ui->ButtonValveStatus1->setText(dev_sta_valve);

            rtu1->data.data.clear();
            char temp_len = data.length();
            QString temp_string = data.toHex().toUpper();
            for(int i = 1; i < temp_len; i++){
                temp_string.insert(3*i-1,' ');
            }
            ui->TextReceive->append(temp_string);
            data.clear();
        }
    }
#endif
}

void MainWindow::serialReadData()
{
    QString str_rec;
    QDataStream readStream(serial);
    for(;;){
        readStream.startTransaction();
        readStream >> str_rec;
        if(readStream.commitTransaction())
            ui->TextReceive->append(str_rec);
        else
            break;
    }
//    QByteArray data;
//    readStream.readBytes(data, receivedNumber);
//    ui->TextReceive->append(QString::fromStdString(data.toStdString()));
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}


void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::show);
    connect(ui->actionInfrared, &QAction::triggered, ir, &Infrared::show);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::ClearAll);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

void MainWindow::timer_irq()
{
    QByteArray data;
    uint16_t crc16;

    static uint8_t irq_counter = 0;

    data.resize(8);
    data[0] = 0x77;
    data[1] = 0x99;
    data[3] = 0x03;
    data[4] = 0x00;
    data[5] = 0x05;

    irq_counter++;

    if(irq_counter % 2) {
        if(ui->CheckDevEnabled1->isChecked()) {
            data[2] = rtu1->data.rtu_address;
        } else {
            return ;
        }
    } else {
        if(ui->CheckDevEnabled->isChecked()) {
            data[2] = rtu->data.rtu_address;
        } else {
            return ;
        }
    }

    QByteArray crc_data = data.mid(2, 4);
    crc16 = rtu->modbus_crc16((unsigned char *)crc_data.data(), 4);
    data[6] = (uint8_t)(crc16 & 0xff);
    data[7] = (uint8_t)(crc16 >> 8);

    QString temp_string = data.toHex().toUpper();
    for(int i = 1; i < data.length(); i++){
        temp_string.insert(3*i-1,' ');
    }
    ui->TextSend->setText(temp_string);

    serial->write(data);
}

void MainWindow::AutoRepeatHandler()
{
    if(ui->CheckAutoRepeat->isChecked()) {
        int delay = ui->SpinMilliSeconds->text().toInt();
        timer->start(delay); // milliseconds
    } else {
        timer->stop();
    }

}

void MainWindow::ForceSetHandler()
{
    if(ui->CheckForceSet->isChecked()) {

    } else {

    }

}

void MainWindow::ForceSetHandler1()
{
    if(ui->CheckForceSet1->isChecked()) {

    } else {

    }

}

void MainWindow::FreshMainWindow(fresh_window_state state)
{
    switch (state) {
    case FRESH_TEXT_RECEIVED:

        break;
    case FRESH_TEXT_SEND:

        break;
    case FRESH_MODE:

        break;
    case FRESH_FAN:

        break;
    case FRESH_TEMP_SETTING:

        break;
    case FRESH_TEMP_ROOM:

        break;
    default:
        break;
    }
//    ui->
}

void MainWindow::FreshMode()
{
    QString state_mod;

    switch (rtu->mode.mod) {
    case 0:
        state_mod = "制冷 Cool";
        break;
    case 1:
        state_mod = "制热 Warm";
        break;
    case 2:
        state_mod = "送风 Fan";
        rtu->mode.wind_auto = 0;
        rtu->mode.wind = FAN_MIN;
        break;
    case 3:

        break;
    case 4:

        break;
    default:
        break;
    }

    if(ui->CheckForceSet->isChecked()) {
        ModbusSend();
    }

    ui->LabelShowMode->setText(state_mod);
}

void MainWindow::FreshFan()
{
    uint8_t dev_sta_wind = 0;
    QString temp_fan_str;

    switch (rtu->mode.wind) {
    case 0:
        dev_sta_wind = 0;
        temp_fan_str = "close";
        break;
    case 1:
        dev_sta_wind = 25;
        temp_fan_str = "low";
        break;
    case 2:
        dev_sta_wind = 50;
        temp_fan_str = "medium";
        break;
    case 3:
        dev_sta_wind = 100;
        temp_fan_str = "high";
        break;
    default:
        break;
    }

    if(rtu->g_fan_ultralow) {
        temp_fan_str = "ultralow";
    }

    ui->BarFanState->setValue(dev_sta_wind);
    ui->BarFanState->setFormat(temp_fan_str);

    if(rtu->mode.wind_auto) {
        ui->LabelFanAuto->show();
    } else {
        ui->LabelFanAuto->hide();
    }
}

void MainWindow::FreshTempSet()
{
    ui->LcdSetTemp->display(rtu->set_temp);

    if(ui->CheckForceSet->isChecked()) {
        ModbusSend();
    }
}

void MainWindow::FreshPowerSet()
{
    if(ui->CheckForceSet->isChecked()) {
        ModbusSend();
    }
}

void MainWindow::ModbusSend()
{
    if(rtu->set_temp < 16 || rtu->set_temp > 32) {
        return ;
    }

    QByteArray data;
    data.resize(11);
    data[0] = 0x77;
    data[1] = 0x99;
    data[2] = rtu->data.rtu_address; // slaver address
    data[3] = 0x10; // write
    data[4] = 0x00; // start address
    data[5] = 0x03; // register counter
    data[6] = 0x00; // button value
    data[7] = rtu->set_temp;
    data[8] = rtu->device_status;

    if(ui->CheckFanUltralow->isChecked()) {
        if(rtu->g_fan_ultralow) {
            data[7] = rtu->set_temp | 0x80;
        }
    }

    unsigned char *str_data = (unsigned char *)malloc(12);
    if (str_data != NULL) {
        memcpy(str_data, data.data(), 11);
    }

    uint16_t crc16 = rtu->modbus_crc16(&str_data[2], data[5] + 4); // calculate CRC

    data[9] = (uint8_t)(crc16 & 0xFF);
    data[10] = (uint8_t)(crc16 >> 8);

    serial->write(data);

    free(str_data);
}

void MainWindow::FreshMode1()
{
    QString state_mod;

    switch (rtu1->mode.mod) {
    case 0:
        state_mod = "制冷 Cool";
        break;
    case 1:
        state_mod = "制热 Warm";
        break;
    case 2:
        state_mod = "送风 Fan";
        rtu1->mode.wind_auto = 0;
        rtu1->mode.wind = FAN_MIN;
        break;
    case 3:

        break;
    case 4:

        break;
    default:
        break;
    }

    if(ui->CheckForceSet1->isChecked()) {
        ModbusSend1();
    }

    ui->LabelShowMode1->setText(state_mod);
}

void MainWindow::FreshFan1()
{
    uint8_t dev_sta_wind = 0;
    QString temp_fan_str;

    switch (rtu1->mode.wind) {
    case 0:
        dev_sta_wind = 0;
        temp_fan_str = "close";
        break;
    case 1:
        dev_sta_wind = 25;
        temp_fan_str = "low";
        break;
    case 2:
        dev_sta_wind = 50;
        temp_fan_str = "medium";
        break;
    case 3:
        dev_sta_wind = 100;
        temp_fan_str = "high";
        break;
    default:
        break;
    }

    if(rtu1->g_fan_ultralow) {
        temp_fan_str = "ultralow";
    }

    ui->BarFanState1->setValue(dev_sta_wind);
    ui->BarFanState1->setFormat(temp_fan_str);

    if(rtu1->mode.wind_auto) {
        ui->LabelFanAuto1->show();
    } else {
        ui->LabelFanAuto1->hide();
    }
}

void MainWindow::FreshTempSet1()
{
    ui->LcdSetTemp1->display(rtu1->set_temp);

    if(ui->CheckForceSet1->isChecked()) {
        ModbusSend1();
    }
}

void MainWindow::FreshPowerSet1()
{
    if(ui->CheckForceSet1->isChecked()) {
        ModbusSend1();
    }
}

void MainWindow::ModbusSend1()
{
    if(rtu1->set_temp < 16 || rtu1->set_temp > 32) {
        return ;
    }

    QByteArray data;
    data.resize(11);
    data[0] = 0x77;
    data[1] = 0x99;
    data[2] = rtu1->data.rtu_address; // slaver address
    data[3] = 0x10; // write
    data[4] = 0x00; // start address
    data[5] = 0x03; // register counter
    data[6] = 0x00; // button value
    data[7] = rtu1->set_temp;
    data[8] = rtu1->device_status;

    if(ui->CheckFanUltralow1->isChecked()) {
        if(rtu1->g_fan_ultralow) {
            data[7] = rtu1->set_temp | 0x80;
        }
    }

    unsigned char *str_data = (unsigned char *)malloc(12);
    if (str_data != NULL) {
        memcpy(str_data, data.data(), 11);
    }

    uint16_t crc16 = rtu1->modbus_crc16(&str_data[2], data[5] + 4); // calculate CRC

    data[9] = (uint8_t)(crc16 & 0xFF);
    data[10] = (uint8_t)(crc16 >> 8);

    serial->write(data);

    free(str_data);
}

void MainWindow::update_uart_settings()
{
    ui->BoxUartName->setCurrentText( settings->currentSettings.name );
    ui->BoxBaudRate->setCurrentText( QString::number(settings->currentSettings.baudRate) );
//    ui->BoxDataBits->setCurrentText() = settings->currentSettings.dataBits;
//    ui->BoxCheckSum->setCurrentText() = settings->currentSettings.parity;
//    ui->BoxStopBits->setCurrentText() = settings->currentSettings.stopBits;
//    ui->BoxFlowControl->setCurrentText() = settings->currentSettings.flowControl;
}

void MainWindow::BoxUartNameChangedHandler(int index)
{
    settings->SetSerialPortName(index);
    settings->updateSettings();
}

void MainWindow::BoxBaudRateChangedHandler(int index)
{
    settings->SetSerialPortBaud(index);
    settings->updateSettings();
}

void MainWindow::SpinMilliSecondsChangedHandler(int milliseconds)
{
    if(timer->isActive()) {
        timer->stop();
    }

    if(milliseconds >=80) {
        timer->start(milliseconds >> 1);
    } else {
        ui->SpinMilliSeconds->setValue(80);
        ui->SpinMilliSeconds->setWrapping(true);
//        ui->SpinMilliSeconds->setCursor(QCursor::setPos(0,0));
//        ui->SpinMilliSeconds->setCursor(QSpinBox.lineEdit(); // -> setCursorPosition(QSpinBox::lineEdit() -> text().length());
    }
}

void MainWindow::SpinAddrHandler(int number)
{
    rtu->data.rtu_address = number;
}

void MainWindow::SpinAddrHandler1(int number)
{
    rtu1->data.rtu_address = number;
}

void MainWindow::handler_btn_mode()
{
    if(rtu->mode.mod == (uint8_t)TEMP_MODE_FAN) {
        rtu->mode.mod = (uint8_t)TEMP_MODE_COOL;
    } else {
        rtu->mode.mod++;
    }

    FreshMode();
}

void MainWindow::handler_btn_fan()
{
    if(rtu->mode.wind_auto) {
        rtu->mode.wind_auto = 0;
        rtu->mode.wind = FAN_MIN;

        if(ui->CheckFanUltralow->isChecked()) {
            rtu->g_fan_ultralow = 1;
        } else {
            rtu->g_fan_ultralow = 0;
        }

        if(ui->CheckForceSet->isChecked()) {
            ModbusSend();
        }

        FreshFan();

        return ;
    }

    if(rtu->g_fan_ultralow) {
        rtu->g_fan_ultralow = 0;
        rtu->mode.wind = FAN_MIN;
        if(ui->CheckForceSet->isChecked()) {
            ModbusSend();
        }

        FreshFan();
        return ;
    }

    if(rtu->mode.wind == (uint8_t)FAN_MAX) {
        rtu->mode.wind =  (uint8_t)FAN_CLZ;
    } else {
        rtu->mode.wind++;
    }

    if(rtu->mode.wind == FAN_CLZ) {
        if(rtu->mode.mod == TEMP_MODE_FAN) {
            rtu->mode.wind = FAN_MIN;
            rtu->mode.wind_auto = 0;
        } else {
            rtu->mode.wind_auto = 1;
        }
    } else {
        rtu->mode.wind_auto = 0;
    }

    if(ui->CheckFanUltralow->isChecked()) {
        if(rtu->mode.wind == FAN_MIN) {
            rtu->g_fan_ultralow = 1;
        }
    }

    if(ui->CheckForceSet->isChecked()) {
        ModbusSend();
    }

    FreshFan();
}

void MainWindow::handler_btn_up()
{
    if(rtu->set_temp < 32) {
        rtu->set_temp++;
    }

    FreshTempSet();
}

void MainWindow::handler_btn_down()
{
    if(rtu->set_temp > 16) {
        rtu->set_temp--;
    }

    FreshTempSet();
}

void MainWindow::handler_btn_power()
{
    if(rtu->mode.power == 0) {
        rtu->mode.power = 1;
    } else if(rtu->mode.power == 1) {
        rtu->mode.power = 0;
    }

    FreshPowerSet();
}

// rtu1
void MainWindow::handler_btn_mode1()
{
    if(rtu1->mode.mod == (uint8_t)TEMP_MODE_FAN) {
        rtu1->mode.mod = (uint8_t)TEMP_MODE_COOL;
    } else {
        rtu1->mode.mod++;
    }

    FreshMode1();
}

void MainWindow::handler_btn_fan1()
{
    if(rtu1->mode.wind_auto) {
        rtu1->mode.wind_auto = 0;
        rtu1->mode.wind = FAN_MIN;

        if(ui->CheckFanUltralow1->isChecked()) {
            rtu1->g_fan_ultralow = 1;
        } else {
            rtu1->g_fan_ultralow = 0;
        }

        if(ui->CheckForceSet1->isChecked()) {
            ModbusSend1();
        }

        FreshFan1();

        return ;
    }

    if(rtu1->g_fan_ultralow) {
        rtu1->g_fan_ultralow = 0;
        rtu1->mode.wind = FAN_MIN;
        if(ui->CheckForceSet1->isChecked()) {
            ModbusSend1();
        }

        FreshFan1();
        return ;
    }

    if(rtu1->mode.wind == (uint8_t)FAN_MAX) {
        rtu1->mode.wind =  (uint8_t)FAN_CLZ;
    } else {
        rtu1->mode.wind++;
    }

    if(rtu1->mode.wind == FAN_CLZ) {
        if(rtu1->mode.mod == TEMP_MODE_FAN) {
            rtu1->mode.wind = FAN_MIN;
            rtu1->mode.wind_auto = 0;
        } else {
            rtu1->mode.wind_auto = 1;
        }
    } else {
        rtu1->mode.wind_auto = 0;
    }

    if(ui->CheckFanUltralow1->isChecked()) {
        if(rtu1->mode.wind == FAN_MIN) {
            rtu1->g_fan_ultralow = 1;
        }
    }

    if(ui->CheckForceSet1->isChecked()) {
        ModbusSend1();
    }

    FreshFan1();
}

void MainWindow::handler_btn_up1()
{
    if(rtu1->set_temp < 32) {
        rtu1->set_temp++;
    }

    FreshTempSet1();
}

void MainWindow::handler_btn_down1()
{
    if(rtu1->set_temp > 16) {
        rtu1->set_temp--;
    }

    FreshTempSet1();
}

void MainWindow::handler_btn_power1()
{
    if(rtu1->mode.power == 0) {
        rtu1->mode.power = 1;
    } else {
        rtu1->mode.power = 0;
    }

    FreshPowerSet1();
}

void MainWindow::ir_get_data(QByteArray buf)
{
    ir_data = buf;
}

void MainWindow::ir_send_data()
{
    static uint32_t send_counter = 0;
    uint32_t len = ir_data.length();

    if(send_counter == 0) {
        serial->write(ir_data.constData(), 1024);
    } else {
        serial->write(ir_data.mid(send_counter << 10).constData(), 1024);
    }

    send_counter++;

//    for(uint32_t i=0; i<(len>>10); i++) {
//        serial->write(buf.constData(), 1024);
//        qApp->processEvents();
//        QThread::msleep(100);
//    }

    if(send_counter >= 270) {
        send_counter = 0;
        infrared_exit();
    }
}

void MainWindow::ir_send_cmd()
{
    QByteArray ir_data;
    ir_data.resize(11);
    ir_data[0] = 0x77;
    ir_data[1] = 0x99;
    ir_data[2] = ir->addr;     // slaver address

    if(ir->ir_func == IR_FUNC_SEND) {
        ir_data[3] = FS_WRITE_PART;
    } else if(ir->ir_func == IR_FUNC_RECV) {
        ir_data[3] = FS_READ_PART;
    } else if(ir->ir_func == IR_FUNC_ERASE) {
        ir_data[3] = FS_ERASE_PART;
    }

    ir_data[4] = 0x00;         // start address
    ir_data[5] = 0x03;         // register counter
    ir_data[6] = 0;            // data1
    ir_data[7] = 0;            // data2
    ir_data[8] = 0;            // data3

    unsigned char *str_data = (unsigned char *)malloc(12);
    if (str_data != NULL) {
        memcpy(str_data, ir_data.data(), 11);
    }

    uint16_t crc16 = rtu->modbus_crc16(&str_data[2], ir_data[5] + 4); // calculate CRC

    ir_data[9] = (uint8_t)(crc16 & 0xFF);
    ir_data[10] = (uint8_t)(crc16 >> 8);

    serial->write(ir_data);

    free(str_data);
}

void MainWindow::infrared_enter()
{
    BoxBaudRateChangedHandler(4);
    ui->BoxBaudRate->setCurrentIndex(4);
    ui->BoxBaudRate->update();
    ui->CheckAutoRepeat->setCheckable(true);
    ui->CheckAutoRepeat->setChecked(false);
    openSerialPort();

    ir_send_cmd();
}

void MainWindow::infrared_exit()
{
    BoxBaudRateChangedHandler(1);
    ui->BoxBaudRate->setCurrentIndex(1);
    ui->BoxBaudRate->update();

    emit ir_data_send_ok();
}
