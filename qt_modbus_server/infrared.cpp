#include "infrared.h"
#include "ui_infrared.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>


Infrared::Infrared(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Infrared)
{
    ui->setupUi(this);

    this->rec_cnt = 0;

    this->fp_bin = NULL;

    addr = 0x03;
    ir_responds = 0;

    ir_func = IR_FUNC_NONE;

    this->setWindowModality(Qt::ApplicationModal);      // this freeze the mainwindow

    connect(ui->ButtonStart, &QPushButton::clicked, this, &Infrared::ButtonHandlerStart);
    connect(ui->ButtonExit, &QPushButton::clicked, this, &Infrared::ButtonHandlerExit);
    connect(ui->ButtonFileOpen, &QPushButton::clicked,  this, &Infrared::ButtonHandlerFileOpen);
    connect(ui->RadioRead, &QRadioButton::clicked, this, &Infrared::RadioHandler);
    connect(ui->RadioWrite, &QRadioButton::clicked, this, &Infrared::RadioHandler);
    connect(ui->RadioErase, &QRadioButton::clicked, this, &Infrared::RadioHandler);
}

Infrared::~Infrared()
{
    delete ui;
}

void Infrared::RadioHandler()
{
    if(ui->RadioWrite->isChecked()) {
        ir_func = IR_FUNC_SEND;
    } else if(ui->RadioRead->isChecked()) {
        ir_func = IR_FUNC_RECV;
    } else if(ui->RadioErase->isChecked()) {
        ir_func = IR_FUNC_ERASE;
    }
}

void Infrared::ButtonHandlerStart()
{
    emit InfraredEnter();   // send cmd

    RadioHandler();

    if(ir_func == IR_FUNC_SEND) {

        if(ui->LineSavePath->text().isEmpty() || ui->LineSavePath->text().isNull()) {
            QMessageBox::warning(this, tr("红外数据"), tr("打开文件错误.\n"
                                                            "请选择正确的文件."),
                                               QMessageBox::Ok | QMessageBox::Cancel);
            return ;
        }

        uint8_t *buf = (uint8_t *)malloc(8<<20);

        ui->ButtonStart->setText("正在发送");
        ui->ButtonStart->setEnabled(false);

        fseek(fp_bin, 0, SEEK_SET);
        fread(buf, NUM_IR_ARRAY<<10, 1, fp_bin);

        QByteArray buf_array = QByteArray((char*)buf, NUM_IR_ARRAY<<10);

        emit DataToSend(buf_array);

        free(buf);

    } else if(ir_func == IR_FUNC_RECV) {

        if(ui->LineSavePath->text().isEmpty() || ui->LineSavePath->text().isNull()) {
            QMessageBox::warning(this, tr("红外数据"), tr("打开文件错误.\n"
                                                            "请选择正确的文件."),
                                               QMessageBox::Ok | QMessageBox::Cancel);
            return ;
        }

        ui->ButtonStart->setText("正在接收");
        ui->ButtonStart->setEnabled(false);

    } else if(ir_func == IR_FUNC_ERASE) {

    }
}

void Infrared::ButtonHandlerExit()
{
//    fflush(fp_bin);
    if(fileno(fp_bin) >= 0) {
        fclose(fp_bin);
    }
    rec_cnt = 0;
    ir_func = IR_FUNC_NONE;

    emit InfraredExit();
    close();
}

void Infrared::ButtonHandlerFileOpen()
{
    RadioHandler();

    QFileDialog dialog(this);
    dialog.setDefaultSuffix("bin");
    dialog.setNameFilter("*.bin");
    dialog.setWindowModality(Qt::WindowModal);

    QString fileName;

    if(ir_func == IR_FUNC_RECV) {
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        if (dialog.exec() != QDialog::Accepted)
            return ;
        fileName = dialog.selectedFiles().first();
        fp_bin = fopen(fileName.toLocal8Bit().constData(), "wb");
    } else if(ir_func == IR_FUNC_SEND) {
        dialog.setFileMode(QFileDialog::ExistingFiles);
        if (dialog.exec() != QDialog::Accepted)
            return ;
        fileName = dialog.selectedFiles().first();
        fp_bin = fopen(fileName.toLocal8Bit().constData(), "rb");
    } else {

    }

    ui->LineSavePath->setText(fileName);
    ui->LineSavePath->setReadOnly(true);
}

void Infrared::DataToFile(QByteArray data)
{
    this->rec_cnt += data.length();
    fwrite((uint8_t *)(data.constData()), data.length(), 1, fp_bin);
    if(rec_cnt >= NUM_IR_ARRAY<<10) {
        fflush(fp_bin);
        rec_cnt = 0;
        DataSendOk();
    }
}

void Infrared::DataSendOk()
{
    ui->ButtonStart->setEnabled(true);
    ui->ButtonStart->setText("开始");
    ir_func = IR_FUNC_NONE;
}
