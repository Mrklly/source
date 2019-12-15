
#include "macdialog.h"
#include "ui_macdialog.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>

QT_USE_NAMESPACE


MACDialog::MACDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MACDialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MACDialog::ButtonHandlerOk);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &MACDialog::ButtonHandlerCancel);
    connect(ui->ButtonFileOpen, &QPushButton::clicked,  this, &MACDialog::ButtonHandlerFileOpen);

}

MACDialog::~MACDialog()
{
    delete ui;
}

void MACDialog::ButtonHandlerOk()
{
    if(ui->LineSavePath->text().isEmpty() || ui->LineSavePath->text().isNull()) {
        QMessageBox::warning(this, tr("中控MAC地址生成"), tr("打开文件错误.\n"
                                                           "请选择正确的文件."),
                                           QMessageBox::Ok | QMessageBox::Cancel);
        return ;
    }
    if(ui->LineMacAmount->text().isEmpty() || ui->LineMacAmount->text().isNull()) {
        QMessageBox::warning(this, tr("中控MAC地址生成"), tr("生成MAC地址的数量错误.\n"
                                                           "请输入要生成的MAC地址数量."),
                                           QMessageBox::Ok | QMessageBox::Cancel);
        return ;
    }
    if(ui->LineStartNumber->text().isEmpty() || ui->LineStartNumber->text().isNull()) {
        QMessageBox::warning(this, tr("中控MAC地址生成"), tr("生成MAC地址的起始编号错误.\n"
                                                           "请输入要生成的MAC地址的起始编号."),
                                           QMessageBox::Ok | QMessageBox::Cancel);
        return ;
    }

    QString fileName = ui->LineSavePath->text();
    unsigned int mac_number = ui->LineMacAmount->text().toUInt();
    QString temp = ui->LineStartNumber->text();

    FILE *fp;

    fp = fopen(fileName.toLocal8Bit().constData(), "w");

    unsigned long long base_number = strtol(temp.toLocal8Bit().constData(), NULL, 16);

    for(unsigned int i = 0; i < mac_number; i++) {

        fprintf(fp, "00,01,");
        fprintf(fp, "%012llX", base_number);
        fprintf(fp, ",01,0000\n");

        base_number++;

    }

    fflush(fp);
    fclose(fp);
}

void MACDialog::ButtonHandlerCancel()
{
    close();
}

void MACDialog::ButtonHandlerFileOpen()
{
    QFileDialog dialog(this);
    dialog.setDefaultSuffix("xls");
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return ;
    QString fileName = dialog.selectedFiles().first();

    ui->LineSavePath->setText(fileName);
    ui->LineSavePath->setReadOnly(true);
}
