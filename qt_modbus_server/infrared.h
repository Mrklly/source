#ifndef INFRARED_H
#define INFRARED_H

#include <QDialog>


#define NUM_POWER      2    // On off
#define NUM_MODE       3    // Cold Warm Fan
#define NUM_FAN        3    // Low Mid High
#define NUM_TEMP       15   // 16 to 30

#define NUM_IR_ARRAY   (NUM_POWER * NUM_MODE * NUM_FAN * NUM_TEMP)


typedef enum _ir_function_t_{
    IR_FUNC_NONE  = 0,
    IR_FUNC_SEND  = 1,
    IR_FUNC_RECV  = 2,
    IR_FUNC_ERASE = 3
} ir_function_t;

namespace Ui {
class Infrared;
}

class Infrared : public QDialog
{
    Q_OBJECT

public:
    explicit Infrared(QWidget *parent = nullptr);
    ~Infrared();

    uint8_t addr;

    ir_function_t ir_func;

    uint8_t ir_responds;

public slots:
    void RadioHandler();
    void ButtonHandlerStart();
    void ButtonHandlerExit();
    void ButtonHandlerFileOpen();
    void DataToFile(QByteArray data);
    void DataSendOk();

signals:
    void DataToSend(QByteArray data);
    void InfraredEnter();
    void InfraredExit();

private:
    Ui::Infrared *ui;

    uint32_t rec_cnt;

    FILE *fp_bin;
};

#endif // INFRARED_H
