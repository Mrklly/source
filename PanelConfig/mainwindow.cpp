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
#include "settingsdialog.h"
#include "modbusrtu.h"

#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QColorDialog>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    memset(g_dev_status, 0, sizeof(g_dev_status));

    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    status = new QLabel;
    ui->statusBar->addWidget(status);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_irq()));

    timer_uart = new QTimer(this);
    connect(timer_uart, SIGNAL(timeout()), this, SLOT(irq_timer_uart()));

    panel = new PanelConfig;
    panel->crc_type = CRC8;
    panel->function = FUNC_CONFIG;
    panel->protocol = PROT_PASSIVE;

    ui->CheckAutoRepeat->setChecked(false);
    ui->CheckAutoRepeat->setEnabled(false);
    ui->SpinMilliSeconds->setEnabled(false);

    ui->BoxUartName->addItems(settings->GetSerialPortNameAll());
    ui->BoxBaudRate->setCurrentText(settings->GetSerialPortBaud());

    ui->CheckShowTime->setChecked(true);
    ui->ButtonSend->setEnabled(false);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    ui->LabelPicture->setAlignment(Qt::AlignCenter);

    initActionsConnections();

    connect(settings, SIGNAL(settingdialog_update()), this, SLOT(update_uart_settings()));

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    connect(ui->ButtonSend, &QPushButton::clicked, this, &MainWindow::writeDataFromTextEditSend);

    connect(ui->BoxUartName, SIGNAL(currentIndexChanged(int)), SLOT(BoxUartNameChangedHandler(int)));
    connect(ui->BoxBaudRate, SIGNAL(currentIndexChanged(int)), SLOT(BoxBaudRateChangedHandler(int)));

//    connect(ui->BoxFunction, SIGNAL(currentIndexChanged(int)), SLOT(HandlerBoxFunction(int)));

    connect(ui->RadioConfig, SIGNAL(clicked()), SLOT(HandlerRadioConfig()));
    connect(ui->RadioPolling, SIGNAL(clicked()), SLOT(HandlerRadioPolling()));
    connect(ui->RadioNaming, SIGNAL(clicked()), SLOT(HandlerRadioNaming()));

    connect(ui->BoxPanelName, SIGNAL(currentIndexChanged(int)), SLOT(HandlerBoxPanelname(int)));
    connect(ui->BoxPanelKeyNum, SIGNAL(currentIndexChanged(int)), SLOT(HandlerBoxPanelKeyNum(int)));
    connect(ui->RadioTypeAc, SIGNAL(clicked()), SLOT(HandlerRadioTypeAc()));
    connect(ui->RadioTypeDc, SIGNAL(clicked()), SLOT(HandlerRadioTypeDc()));
    connect(ui->RadioProtocolA, SIGNAL(clicked()), SLOT(HandlerRadioProtocolA()));
    connect(ui->RadioProtocolP, SIGNAL(clicked()), SLOT(HandlerRadioProtocolP()));
    connect(ui->RadioNearTrue, SIGNAL(clicked()), SLOT(HandlerRadioNearTrue()));
    connect(ui->RadioNearFalse, SIGNAL(clicked()), SLOT(HandlerRadioNearFalse()));
    connect(ui->RadioCrc8, SIGNAL(clicked()), SLOT(HandlerRadioCrc8()));
    connect(ui->RadioCrc16, SIGNAL(clicked()), SLOT(HandlerRadioCrc16()));
    connect(ui->RadioCrcXor, SIGNAL(clicked()), SLOT(HandlerRadioCrcXor()));
    connect(ui->SpinPanelAddr, SIGNAL(valueChanged(int)), SLOT(HandlerSpinPanelAddr(int)));

    ui->SpinMilliSeconds->setKeyboardTracking(false);
    connect(ui->SpinMilliSeconds, SIGNAL(valueChanged(int)), SLOT(SpinMilliSecondsChangedHandler(int)));

}


MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}


void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

        if(ui->RadioConfig->isChecked()) {
            panel->function = FUNC_CONFIG;
        } else if(ui->RadioPolling->isChecked()) {
            panel->function = FUNC_TEST_POLLING;
        } else if(ui->RadioNaming->isChecked()) {
            panel->function = FUNC_TEST_NAMING;
        }

        if(panel->function == FUNC_CONFIG) {
            ui->ButtonSend->setEnabled(true);
            ui->CheckAutoRepeat->setChecked(false);
            ui->CheckAutoRepeat->setEnabled(false);
            timer->stop();
        } else {

            int time_interval = 200;

            if(panel->function == FUNC_TEST_POLLING) {
                time_interval = 200;
            } else if(panel->function == FUNC_TEST_NAMING) {
                time_interval = 20;
            }

            ui->SpinMilliSeconds->setValue(time_interval);

            if(panel->protocol == PROT_PASSIVE) {
                ui->ButtonSend->setEnabled(false);
                ui->CheckAutoRepeat->setEnabled(true);
                ui->CheckAutoRepeat->setChecked(true);
                timer->start(ui->SpinMilliSeconds->text().toInt()); // milliseconds
            } else {
                timer->stop();
            }

        }

    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        ui->CheckAutoRepeat->setChecked(false);
        ui->CheckAutoRepeat->setEnabled(false);
        ui->ButtonSend->setEnabled(false);
        showStatusMessage(tr("Open error"));
    }
}


void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    timer->stop();
    timer_uart->stop();
    ui->CheckAutoRepeat->setChecked(false);
    ui->CheckAutoRepeat->setEnabled(false);
    ui->ButtonSend->setEnabled(false);
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
    panel->config.key_number &= ~0xC0;

    if(panel->config.name == QB522) {
        if(panel->config.key_number == 2) {

            if(ui->RadioKeyType2C4K->isChecked()) {
                panel->config.key_number |= (KEY_2_V << 6);
            } else if(ui->RadioKeyType2C6K->isChecked()) {
                panel->config.key_number |= (KEY_2_6 << 6);
            } else {
                return ;
            }

        }
    }

    if(panel->crc_type == CRC8) {
        panel->config.crc = panel->get_crc8(&(panel->config.head), 4);
    } else if(panel->crc_type == CRC_XOR) {
        panel->config.crc = panel->get_crc_xor(&(panel->config.head), 4);
    }

    char buf_tmp[sizeof(panel->config)];
    memcpy(buf_tmp, &panel->config, sizeof(panel->config));
    QByteArray data = QByteArray::fromRawData(buf_tmp, sizeof(panel->config));

    QString str = data.toHex().toUpper();
    for(int i = 1; i < data.length(); i++){
        str.insert(3*i-1,' ');
    }
    qDebug() << "str: " << str;
    ui->TextSend->setText(str);

    serial->write(data);
}


void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    uint8_t crc = 0;
    static uint8_t buf_send[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    data.append(serial->readAll());

    if(panel->function == FUNC_CONFIG) {
        data_show.append(data);
        data.clear();
    } else {
        if(data.length() >= 3) {
            if(panel->crc_type == CRC_XOR) {
                crc = panel->get_crc_xor((unsigned char *)data.data(), 2);
            } else if(panel->crc_type == CRC8) {
                crc = panel->get_crc8((unsigned char *)data.data(), 2);
            }
            if(crc == (uint8_t)data.at(2)) {
                buf_send[0] = (uint8_t)((data[0] - 0x10) / 6) + 0xD8;

                for(uint8_t i=0; i<6; i++) {
                    if(data[1] & (1 << i)) {
                        if(g_dev_status[data[0]] & (1 << i)) {
                            g_dev_status[data[0]] &= ~(1 << i);
                        } else {
                            g_dev_status[data[0]] |=  (1 << i);
                        }
                    }
                }

                for(uint8_t i=1; i<7; i++) {
                    buf_send[i] = g_dev_status[((data[0] - 0x10) / 6) * 6 + 0x10 + i - 1];
                }

                if(panel->crc_type == CRC_XOR) {
                    buf_send[7] = panel->get_crc_xor(buf_send, 7);
                } else if(panel->crc_type == CRC8){
                    buf_send[7] = panel->get_crc8(buf_send, 7);
                }

                serial->write((const char*)buf_send, 8);

            }
            data_show.append(data);
            data.clear();
        }

    }

    timer_uart->start(20);

//    QDateTime *datatime = new QDateTime();
//    uint64_t time_now = datatime->currentMSecsSinceEpoch();


//    static uint64_t time_last = 0;

//    qDebug() << "time: " << time_now;

//    if(time_now - time_last < 20) {
//        if(time_last != 0) {
//            return ;
//        }
//    }

//    time_last = time_now;

//    delete datatime;
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

    static uint8_t panel_addr = PANEL_ADDR_BASE;

    data.resize(3);

    if(panel->function == FUNC_TEST_POLLING) {
        data[0] = 0x74;
        data[1] = 0xAA;
    } else if(panel->function == FUNC_TEST_NAMING) {
        data[0] = 0x75;

        data[1] = panel_addr++;

        if(panel_addr == PANEL_NUMBER_MAX + PANEL_ADDR_BASE) {
            panel_addr = PANEL_ADDR_BASE;
        }
    }

    if(panel->crc_type == CRC8) {
        data[2] = panel->get_crc8((unsigned char *)data.data(), 2);
    } else if(panel->crc_type == CRC_XOR) {
        data[2] = panel->get_crc_xor((unsigned char *)data.data(), 2);
    } else {
        ui->TextSend->clear();
        return ;
    }

    QString temp_string = data.toHex().toUpper();
    for(int i = 1; i < data.length(); i++){
        temp_string.insert(3*i-1,' ');
    }
    ui->TextSend->setText(temp_string);

    serial->write(data);
}

void MainWindow::irq_timer_uart()
{
    timer_uart->stop();

    char temp_len = data_show.length();
    QString str_time = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");
    QString str_data = data_show.toHex().toUpper();

    if(ui->CheckShowTime->isChecked() == false) {
        str_time.clear();
    }

    for(int i = 1; i < temp_len; i++){
        str_data.insert(3*i-1,' ');
    }
    ui->TextReceive->append(str_time + str_data);
    data_show.clear();
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

void MainWindow::ModbusSend()
{

    QByteArray data;
    data.resize(11);
    data[0] = 0x77;
    data[1] = 0x99;
    data[2] = 0;
    data[3] = 0x10;
    data[4] = 0x00;


    unsigned char *str_data = (unsigned char *)malloc(12);
    if (str_data != NULL) {
        memcpy(str_data, data.data(), 11);
    }

    uint16_t crc16 = panel->modbus_crc16(&str_data[2], data[5] + 4); // calculate CRC

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

void MainWindow::HandlerRadioConfig()
{
    HandlerBoxFunction(FUNC_CONFIG);
}

void MainWindow::HandlerRadioPolling()
{
    HandlerBoxFunction(FUNC_TEST_POLLING);
}

void MainWindow::HandlerRadioNaming()
{
    HandlerBoxFunction(FUNC_TEST_NAMING);
}

void MainWindow::HandlerBoxFunction(int index)
{
    if( (serial == NULL) || !(serial->isOpen() )) {
        QMessageBox::warning(this, tr("Function"), tr("串口未打开.\n"
                                                     "请点击打开串口."),QMessageBox::Ok | QMessageBox::Cancel);
        return ;
    }

    panel->function = (function_t)index;
    ui->TextSend->clear();

    if(index == FUNC_CONFIG) {
        timer->stop();
        ui->CheckAutoRepeat->setChecked(false);
        ui->CheckAutoRepeat->setEnabled(false);
        ui->SpinMilliSeconds->setEnabled(false);
        ui->ButtonSend->setEnabled(true);
    } else {
        if(ui->RadioProtocolA->isChecked()) {
            return ;
        }

        int time_interval = 200;

        if(panel->function == FUNC_TEST_POLLING) {
            time_interval = 200;
        } else if(panel->function == FUNC_TEST_NAMING) {
            time_interval = 20;
        }

        ui->SpinMilliSeconds->setValue(time_interval);

        timer->start(time_interval); // milliseconds

        ui->CheckAutoRepeat->setEnabled(true);
        ui->CheckAutoRepeat->setChecked(true);
        ui->SpinMilliSeconds->setEnabled(true);
        ui->ButtonSend->setEnabled(false);
    }
}

void MainWindow::HandlerBoxPanelname(int index)
{
    QString str;

    panel->config.name = (panel_name_t)index;

    if(panel->config.name != QB522) {
        ui->RadioKeyType2C4K->setChecked(false);
        ui->RadioKeyType2C6K->setChecked(false);

        ui->RadioKeyType2C4K->setEnabled(false);
        ui->RadioKeyType2C6K->setEnabled(false);
    } else {
        ui->RadioKeyType2C4K->setEnabled(true);
        ui->RadioKeyType2C6K->setEnabled(true);
    }

    if(index == 0 || panel->config.key_number == 0) {
        ui->LabelPicture->clear();
        return ;
    }

    const QString str_con = ":/images/panel/%1%2.jpg";
    str = str_con.arg(ui->BoxPanelName->currentText()).arg(panel->config.key_number);
    qDebug() << "str: " << str;

    QPixmap pic;
    if(pic.load(str) == 0) {
        ui->LabelPicture->clear();
        qDebug() << "pic is null";
        return ;
    }
    pic = pic.scaled(ui->LabelPicture->size(), Qt::KeepAspectRatio);

    ui->LabelPicture->setPixmap(pic);
}

void MainWindow::HandlerBoxPanelKeyNum(int index)
{
    QString str;

    panel->config.key_number = index;

    if(index == 0 || ui->BoxPanelName->currentIndex() == 0) {
        ui->LabelPicture->clear();
        return ;
    }

    const QString str_con = ":/images/panel/%1%2.jpg";
    str = str_con.arg(ui->BoxPanelName->currentText()).arg(panel->config.key_number);
    qDebug() << "str: " << str;

    QPixmap pic;
    if(pic.load(str) == 0) {
        ui->LabelPicture->clear();
        qDebug() << "pic is null";
        return ;
    }
    pic = pic.scaled(ui->LabelPicture->size(), Qt::KeepAspectRatio);

    ui->LabelPicture->setPixmap(pic);
}

void MainWindow::HandlerRadioTypeAc()
{
    ui->RadioProtocolA->setChecked(true);
    ui->RadioProtocolP->setChecked(false);
    ui->RadioProtocolP->setEnabled(false);

    ui->RadioNearFalse->setChecked(true);
    ui->RadioNearTrue->setChecked(false);
    ui->RadioNearTrue->setEnabled(false);
}

void MainWindow::HandlerRadioTypeDc()
{
    ui->RadioNearTrue->setEnabled(true);

    ui->RadioProtocolA->setChecked(false);
    ui->RadioProtocolP->setEnabled(true);
    ui->RadioProtocolP->setChecked(true);
}

void MainWindow::HandlerRadioProtocolP()
{
    panel->protocol = PROT_PASSIVE;
    ui->CheckAutoRepeat->setChecked(true);
}

void MainWindow::HandlerRadioProtocolA()
{
    panel->protocol = PROT_ACTIVE;
    ui->CheckAutoRepeat->setChecked(false);
}

void MainWindow::HandlerRadioNearTrue()
{
//    panel->config.misc |= 0x80;     // bit7 used for near sensor config
    panel->config.near_sen = 1;
}

void MainWindow::HandlerRadioNearFalse()
{
//    panel->config.misc &= ~0x80;
    panel->config.near_sen = 0;
}

void MainWindow::HandlerRadioCrc8()
{
    panel->crc_type = CRC8;
}

void MainWindow::HandlerRadioCrc16()
{
    panel->crc_type = CRC16;
}

void MainWindow::HandlerRadioCrcXor()
{
    panel->crc_type = CRC_XOR;
}

void MainWindow::HandlerSpinPanelAddr(int value)
{
    panel->config.addr = value;
}

void MainWindow::SpinMilliSecondsChangedHandler(int milliseconds)
{
    if(timer->isActive()) {
        timer->stop();
    }

    if(milliseconds >= 10) {
        timer->start(milliseconds);
    } else {
        ui->SpinMilliSeconds->setValue(10);
        ui->SpinMilliSeconds->setWrapping(true);
    }
}
