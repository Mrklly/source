#ifndef MACDIALOG_H
#define MACDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
class MACDialog;
}
QT_END_NAMESPACE


class MACDialog : public QDialog
{
    Q_OBJECT

public:

    explicit MACDialog(QWidget *parent = nullptr);
    ~MACDialog();

public slots:
    void ButtonHandlerOk();
    void ButtonHandlerCancel();
    void ButtonHandlerFileOpen();

//private:

private:
    Ui::MACDialog *ui;

signals:
    void dialog_update();
};




#endif // MACDIALOG_H
