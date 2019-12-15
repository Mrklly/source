#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include "iniparser/iniparser.h"
#include "iniparser/iniparsertest.h"

static int callback_create_insert(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int callback_check_table_exists(void *data, int argc, char **argv, char **azColName)
{
    char **result_str = (char **)data;
    *result_str = (char *)calloc(strlen(argv[0]), sizeof(char));
    strcpy(*result_str, argv[0]);
    return 0;
}


int RunSqlNoCallback(sqlite3 * db, const char * zSql)
{
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, zSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        return rc;

    int rowCount = 0;
    rc = sqlite3_step(stmt);
    while (rc != SQLITE_DONE && rc != SQLITE_OK)
    {
        rowCount++;
        int colCount = sqlite3_column_count(stmt);
        for (int colIndex = 0; colIndex < colCount; colIndex++)
        {
            int type = sqlite3_column_type(stmt, colIndex);
            const char * columnName = sqlite3_column_name(stmt, colIndex);
            if (type == SQLITE_INTEGER)
            {
                int valInt = sqlite3_column_int(stmt, colIndex);
                printf("columnName = %s, Integer val = %d\n", columnName, valInt);
            }
            else if (type == SQLITE_FLOAT)
            {
                double valDouble = sqlite3_column_double(stmt, colIndex);
                printf("columnName = %s,Double val = %f\n", columnName, valDouble);
            }
            else if (type == SQLITE_TEXT)
            {
                const unsigned char * valChar = sqlite3_column_text(stmt, colIndex);
                printf("columnName = %s,Text val = %s\n", columnName, valChar);
                free((void *)valChar);
            }
            else if (type == SQLITE_BLOB)
            {
                printf("columnName = %s,BLOB\n", columnName);
            }
            else if (type == SQLITE_NULL)
            {
                printf("columnName = %s,NULL\n", columnName);
            }
        }
        printf("Line %d, rowCount = %d\n", rowCount, colCount);

        rc = sqlite3_step(stmt);
    }

    rc = sqlite3_finalize(stmt);

    return rc;
}

void initializeModel(QStandardItemModel *model)
{
    model->setColumnCount(4);
//    model->setRowCount(10);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("设备MAC地址"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("设备名称"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("路数"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("状态"));
}

QTableView *createView(QStandardItemModel *model, const QString &title = "")
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    return view;
}

void MainWindow::showEvent( QShowEvent* event ) {
    QMainWindow::showEvent( event );

    static uint8_t first_open = 0;

    if(first_open) {
        return ;
    }

    QTimer::singleShot(50, this, SLOT(check_ini()));

    first_open = 1;
//        QCoreApplication::exit(-1);
//        QApplication::exit(-1);
//        QWidget::close();
//        QApplication::closeAllWindows();
//        qApp->exit();
}

void MainWindow::check_ini()
{
    dictionary *ini;
    ini = iniparser_load( "archive.ini" );
    if ( ini== NULL) {
//        fprintf(stderr, "cannot parse file: %s\n", "archive.ini");
//        fflush(stderr);

        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("打开配置文件"),
                                   tr("找不到配置文件，\n"
                                      "这是第一次生产, \n"
                                      "设备ID将从800开始！"),
                                   QMessageBox::Ok | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Ok:

            create_ini_file();
            ui->LineEditNewOrder->setText(tr("800"));

            break;
        case QMessageBox::Cancel:
            QCoreApplication::exit(-1);
            break;
        default:
            break;
        }
    } else {
        ui->LineEditNewOrder->setText(tr("0"));

        int device_order = 0, device_counter = -1;
        QString temp;
        const char * dev_name ;

//        for(uint8_t i=0; i<ini->n; i++) {
//            qDebug() << QString::fromLocal8Bit(ini->key[i]);
//        }

        int dev_num = iniparser_getsecnkeys(ini, "status");

#if _DEBUG
        const char *keys[32];
        for(uint8_t i=0; i<dev_num; i++) {
            iniparser_getseckeys(ini, "status", keys);
            qDebug() << QString::fromLocal8Bit(keys[i]);
        }

        qDebug() << "ini string:";
#endif

        for(int i = 0; i < device_type_group.length(); i++) {
            temp = "Status:" + device_type_group.at(i);

            device_counter = iniparser_getint(ini, temp.toLocal8Bit().data(), NULL);
            if(device_counter == 0) {
                device_order = i;
                if(device_order != 0) {
                    FILE *fp;
                    temp = "Status:" + device_type_group.at(device_order);
                    iniparser_set(ini, temp.toLocal8Bit().data(), "800");
                    fp = fopen("archive.ini", "w");
                    iniparser_dump_ini(ini, fp);
                    fclose(fp);
                }
                device_counter = 800;
                QString temp_string = "软件已更新! 增加 " + device_type_group.at(i) + "设备";
                QMessageBox::warning(this, tr("加载配置文件"), temp_string, QMessageBox::Ok);
            }
        }

        if(device_order != 0) {
            FILE *fp;
            temp = "Status:" + device_type_group.at(device_order);
            iniparser_set(ini, temp.toLocal8Bit().data(), "800");
            fp = fopen("archive.ini", "w");
            iniparser_dump_ini(ini, fp);
            fclose(fp);
        }
    }
    iniparser_freedict(ini);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MACgenerator = new MACDialog;
    device_type_group << "PowerControl";
    device_type_group << "SecuritySafe";
    device_type_group << "WindowControl";
    device_type_group << "CurtainControl";
    device_type_group << "EntranceGuard";
    device_type_group << "MediaControl";
    device_type_group << "AirConditionerControl";
    device_type_group << "VentilationControl";
    device_type_group << "HeatingControl";
    device_type_group << "DoorMagneticInduction";
    device_type_group << "InfraredInduction";
    device_type_group << "SmokeInduction";
    device_type_group << "CombustibleGasInduction";
    device_type_group << "CarbonMonoxideInduction";
    device_type_group << "WaterImmersionInduction";
    device_type_group << "AlarmKey";
    device_type_group << "AlarmTempHumidity";
    device_type_group << "AlarmSoundLight";
    device_type_group << "AlarmRemoteControl";
    device_type_group << "DimmerControl";
    device_type_group << "ScenePanel";
    device_type_group << "WirelessRepeater";
    device_type_group << "IrRepeater";


//    QObject::connect(this ,ui->ButtonSerialSend,this,&MainWindow::on_SendButton_clicked);
    connect(ui->ButtonSerialRefresh, &QPushButton::clicked, this, &MainWindow::ButtonSerialRefresh_clicked);
    connect(ui->ButtonSerialOpen, &QPushButton::clicked, this, &MainWindow::ButtonSerialOpen_clicked);
    connect(ui->ButtonSerialSend, &QPushButton::clicked, this, &MainWindow::ButtonSerialSend_clicked);
    ui->ButtonSerialSend->setEnabled(false);

    connect(ui->ButtonSerialReceiveClear, &QPushButton::clicked, this, &MainWindow::ButtonSerialReceiveClearClicked);

    connect(ui->ButtonDeviceAdd,    &QPushButton::clicked, this, &MainWindow::ButtonDeviceAddHandler);
    connect(ui->ButtonDeviceDelete, &QPushButton::clicked, this, &MainWindow::ButtonDeviceDeleteHandler);

    connect(ui->ButtonSetbyHand, &QPushButton::clicked, this, &MainWindow::ButtonSetbyHandHandler);
    connect(ui->ButtonSetAuto, &QPushButton::clicked, this, &MainWindow::ButtonSetAutoHandler);

    connect(ui->ActionFileNew, &QAction::triggered, this, &MainWindow::ActionFileNewHandler);
    connect(ui->ActionFileOpen, &QAction::triggered, this, &MainWindow::ActionFileOpenHandler);
//    connect(ui->ActionFileSave, &QAction::triggered, this, &MainWindow::ActionFileSaveHandler);
    connect(ui->ActionFileExit, &QAction::triggered, this, &MainWindow::ActionFileExitHandler);
    connect(ui->ActionHelp,     &QAction::triggered, this, &MainWindow::ActionFileHelpHandler);
    connect(ui->ActionTest,     &QAction::triggered, this, &MainWindow::ActionFileTestHandler);
//    connect(ui->ActionMSMAC,    &QAction::triggered, this, &MainWindow::ActionMSMACHandler);
    connect(ui->ActionMSMAC,    &QAction::triggered, MACgenerator, &MACDialog::show);

    connect(ui->BoxNewType, SIGNAL(currentIndexChanged(int)), SLOT(BoxTypeChangedHandler(int)));
    ButtonSerialRefresh_clicked();

    ui->BoxBaudrate->setCurrentIndex(0);

    model = new QStandardItemModel();
    initializeModel(model);

    db = NULL;
    serial = NULL;

    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setHidden(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setColumnWidth(4, 150);
    ui->tableView->setVerticalScrollBar(new QScrollBar(ui->tableView));
    ui->tableView->setMouseTracking(true);
    ui->tableView->show();

//    for(int i = 0; i < model->rowCount(); i++) {
//        for(int j = 0; j < model->columnCount(); j++) {
//            model->setData(model->index(i, j), j+i);
//        }
//    }

    deviceinfo = new DeviceInfo();
    TimerSerialLineWrap = new QTimer(this);
    connect(TimerSerialLineWrap, SIGNAL(timeout()), this, SLOT(TimerSerialLineWrapHandler()));

    ActionFileTestHandler();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TimerSerialLineWrapHandler()
{
    TimerSerialLineWrap->stop();
//    ui->TextEditReceive->append("\n");
    QTextEdit *myTextEdit = ui->TextEditReceive;
    myTextEdit->moveCursor (QTextCursor::End);
    myTextEdit->insertPlainText ("\n");
}

void MainWindow::ButtonSerialRefresh_clicked()
{
    ui->BoxComPort->clear();
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->BoxComPort->addItem(serial.portName());
            serial.close();
        }
    }
}

void MainWindow::ButtonSerialOpen_clicked()
{
    ui->ButtonSetbyHand->setStyleSheet("");
    ui->ButtonSetAuto->setStyleSheet("");
    if(ui->ButtonSerialOpen->text() == tr("打开串口")) {
        serial = new QSerialPort;

        if(ui->BoxComPort->currentText().isEmpty() || ui->BoxComPort->currentText().isNull()) {
            QMessageBox::warning(this, tr("打开串口"), tr("串口号为空，请点击刷新串口!"),
                                       QMessageBox::Ok | QMessageBox::Cancel);
            return ;
        }

        serial->setPortName(ui->BoxComPort->currentText());

        serial->open(QIODevice::ReadWrite);

        serial->setBaudRate(ui->BoxBaudrate->currentText().toInt());

        switch (ui->BoxDataBit->currentIndex()) {
            case 8:
                serial->setDataBits(QSerialPort::Data8);
                break;
            default:
                break;
        }

        switch (ui->BoxParity->currentIndex()) {
            case 0:
                serial->setParity(QSerialPort::NoParity);
                break;
            default:
                break;
        }

        switch (ui->BoxStopBit->currentIndex()) {
            case 1:
                serial->setStopBits(QSerialPort::OneStop);
                break;
            case 2:
                serial->setStopBits(QSerialPort::TwoStop);
            default:
                break;
        }

        serial->setFlowControl(QSerialPort::NoFlowControl);

        ui->BoxComPort->setEnabled(false);
        ui->BoxBaudrate->setEnabled(false);
        ui->BoxDataBit->setEnabled(false);
        ui->BoxParity->setEnabled(false);
        ui->BoxStopBit->setEnabled(false);
        ui->ButtonSerialOpen->setText(tr("关闭串口"));
        ui->ButtonSerialSend->setEnabled(true);
        ui->ButtonSerialRefresh->setEnabled(false);

        QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::ReadData);
    } else {

        serial->clear();
        serial->close();
        serial->deleteLater();
        serial = NULL;
        ui->ButtonSetAuto->setEnabled(true);
        ui->ButtonSetAuto->setText(tr("开始自动配置"));
        deviceinfo->SetAutoFlag = 0;
        ui->ButtonSetbyHand->setEnabled(true);
        ui->ButtonSetbyHand->setText(tr("手动配置"));
        deviceinfo->SetByHandFlag = 0;

        ui->BoxComPort->setEnabled(true);
        ui->BoxBaudrate->setEnabled(true);
        ui->BoxDataBit->setEnabled(true);
        ui->BoxParity->setEnabled(true);
        ui->BoxStopBit->setEnabled(true);
        ui->ButtonSerialOpen->setText(tr("打开串口"));
        ui->ButtonSerialSend->setEnabled(false);
        ui->ButtonSerialRefresh->setEnabled(true);
    }
}

void MainWindow::ReadData()
{
    static uint8_t set_status = 0;
    static QByteArray buf;
    QModelIndex index;
    int rc = 0;
    const char *zSQL = 0;
    unsigned int row_counter = 0;
    buf.append(serial->readAll());

    if(deviceinfo->SetByHandFlag) {
        row_counter = ui->tableView->selectionModel()->selectedRows().at(0).row();
    } else if(deviceinfo->SetAutoFlag) {
        row_counter = deviceinfo->device_been_set;
    }

    if((row_counter != 0) && (model->rowCount() != 0) && (row_counter >= model->rowCount())) {
        ui->ButtonSetAuto->setText("开始自动配置");
        ui->ButtonSetAuto->setStyleSheet("");
        ui->ButtonSetbyHand->setEnabled(true);
        deviceinfo->SetAutoFlag = 0;
        QMessageBox::warning(this, tr("配置中"), tr("当前数据库已全部配置完成.\n"
                                                   "请添加设备或者新建数据库."), QMessageBox::Ok | QMessageBox::Cancel);
    }

    index = model->index(row_counter, 0, QModelIndex());
    QString temp_id =  model->data(index).toString();
    QByteArray device_mac_array = QByteArray::fromHex( temp_id.toLatin1() );
    memcpy(&(deviceinfo->currentSettings.name[10]), device_mac_array, device_mac_array.length());

    index = model->index(row_counter, 2, QModelIndex());
    deviceinfo->currentSettings.channel = model->data(index).toString().toUInt();

    if(((deviceinfo->SetByHandFlag)) || ((deviceinfo->SetAutoFlag))) {
        if(buf.size() == 1) {
            if((unsigned char)buf.at(0) == 0xAA) {    // received mcu request
                set_status = 1;
                serial->write("U");   // 0x55
                const char *p = (const char *)(&(deviceinfo->currentSettings.SOH));
                serial->write(p, 89);
            }
        }
    }

    if(set_status) {
        if((unsigned char)buf.at(0) == 0x01) {
            if(buf.size() < 4) {
                return ;
            } else if((buf.at(1) == 0) && (buf.at(2) == 0) && (buf.at(3) == 0x01)) {
                set_status = 0;
                model->setData(model->index(row_counter, 3), QString::fromUtf8("已配置"));

                if(ui->ActionTest->isChecked() == 0) {
                    QString id_str = model->data(model->index(row_counter,0)).toString();
                    zSQL = sqlite3_mprintf("UPDATE DEVICE set STATUS = '已配置' where ID = '%q';", id_str.toStdString().c_str());
                    rc = sqlite3_exec(db, zSQL, callback_check_table_exists, NULL, NULL);
                    if (rc != SQLITE_OK)
                        return ;
                }

                if(deviceinfo->SetByHandFlag) {
                    deviceinfo->SetByHandFlag = 0;
                    ui->ButtonSetbyHand->setStyleSheet("");
                    ui->ButtonSetbyHand->setText("手动配置");
                    ui->ButtonSetbyHand->setEnabled(true);
                    ui->ButtonSetAuto->setEnabled(true);
                } else if(deviceinfo->SetAutoFlag) {
                    ui->tableView->selectRow(row_counter+1);
                    if(row_counter % 12 == 0) {
                        ui->tableView->scrollTo(model->index(row_counter + 12, 0));
                    }
//                    if(ui->ActionTest->isChecked() == 0) {
                        deviceinfo->device_been_set++;
//                    }
                }
            }
        }
    }

    if(!buf.isEmpty()) {
        QString str = ui->TextEditReceive->toPlainText();
        buf = buf.toHex().toUpper();
        buf.resize(3*buf.length());
        for(int i = 0; i < buf.length()/3; i++) {
            buf.insert(3*i+2, ' ');
        }
        str+=tr(buf);
        ui->TextEditReceive->clear();
        ui->TextEditReceive->append(str);
        TimerSerialLineWrap->start(50);
    }
    buf.clear();
}

void MainWindow::ButtonSerialSend_clicked()
{
//    serial->write(ui->TextEditSend->toPlainText().toLatin1());    // ASCII send

    QString str = ui->TextEditSend->toPlainText();
    QByteArray data = QByteArray::fromHex( str.toLatin1() );
    serial->write(data);
}

void MainWindow::ButtonSerialReceiveClearClicked()
{
    ui->TextEditReceive->clear();
}

void MainWindow::SetDeviceInfo()
{
    deviceinfo->currentSettings.channel = 4;

    deviceinfo->currentSettings.ctrl_type[0] = 0;
    deviceinfo->currentSettings.ctrl_type[1] = 1;
    deviceinfo->currentSettings.ctrl_type[2] = 2;
    deviceinfo->currentSettings.ctrl_type[3] = 3;

    deviceinfo->currentSettings.name[0] = 's';  // name 0~5, device name
    deviceinfo->currentSettings.name[1] = 'w';
    deviceinfo->currentSettings.name[2] = 'i';
    deviceinfo->currentSettings.name[3] = 't';
    deviceinfo->currentSettings.name[4] = 'c';
    deviceinfo->currentSettings.name[5] = 'h';

    deviceinfo->currentSettings.name[6] = 0x01;  // name 6~9, key order number
    deviceinfo->currentSettings.name[7] = 0x02;
    deviceinfo->currentSettings.name[8] = 0x04;
    deviceinfo->currentSettings.name[9] = 0x08;

    for(int i = 0; i < 64; i++) {
        deviceinfo->currentSettings.dat[i] = i;
    }


//    const char *p = (const char *)(&(deviceinfo->currentSettings.SOH));

//    serial->write(p, 89);


}

void MainWindow::ButtonDeviceAddHandler()
{
    if(ui->LineEditNewName->text().isEmpty()) {
        QMessageBox::critical(this, tr("Application Name"), tr("设备名不能为空.") );
        return ;
    } else if (ui->LineEditNewName->text().count() >3) {
        QMessageBox::critical(this, tr("Application Name"), tr("设备名不能超过三个字节.") );
        return ;
    }
    if(ui->LineEditNewNumber->text().isEmpty()) {
        QMessageBox::critical(this, tr("Application Name"), tr("数量不能为空.") );
        return ;
    }
    if(ui->BoxNewChannel->currentIndex() == 0) {
        QMessageBox::critical(this, tr("Application Name"), tr("路数不能为空.") );
        return ;
    }
    if(ui->BoxNewType->currentIndex() == 0) {
        QMessageBox::critical(this, tr("Application Name"), tr("类型不能为空.") );
        return ;
    }

    if(db == NULL) {
        QMessageBox MessageBoxAddDevice(this);
        MessageBoxAddDevice.setIcon(QMessageBox::Warning);
        MessageBoxAddDevice.setWindowTitle(tr("新增设备"));
        MessageBoxAddDevice.setText(tr("当前未选中数据库文件,请选择你需要执行的操作。\n"
                                       "新建数据库： 新建一个.db结尾的数据库文件并保存到硬盘。\n"
                                       "打开数据库： 从硬盘打开一个以.db结尾的数据库文件，并载入到表格。\n"
                                       " 取  消：       取消新增设备的操作。"));
        QAbstractButton* ButtonNew = MessageBoxAddDevice.addButton(tr("新建数据库"), QMessageBox::NoRole);
        QAbstractButton* ButtonOpen = MessageBoxAddDevice.addButton(tr("打开数据库"), QMessageBox::NoRole);
        QAbstractButton* ButtonCancel = MessageBoxAddDevice.addButton(tr("取消"), QMessageBox::NoRole);
        MessageBoxAddDevice.exec();
        if(MessageBoxAddDevice.clickedButton() == ButtonNew) {
            ActionFileNewHandler();
            if(db == NULL) {
                return ;
            }
        } else if(MessageBoxAddDevice.clickedButton() == ButtonOpen) {
            ActionFileOpenHandler();
            if(db == NULL) {
                return ;
            }
        } else if(MessageBoxAddDevice.clickedButton() == ButtonCancel) {
            return ;
        }
    } else {

    }

    uint8_t device_mac_order, device_mac_prefix;

    device_mac_order = ui->BoxNewType->currentIndex();

    if(       device_mac_order == 0x14) {
        device_mac_prefix = 0x19;
    } else if(device_mac_order > 0x14) {
        device_mac_prefix = device_mac_order + 11;
    } else {
        device_mac_prefix = device_mac_order;
    }

    QString device_mac = ui->LineEditNewOrder->text();
    unsigned long long device_mac_number = device_mac.toULongLong(NULL, 16);
    device_mac = QString("%1").arg(device_mac, 12, QChar('0'));
    QByteArray device_mac_array = QByteArray::fromHex( device_mac.toLatin1() );
    device_mac_array[0] = device_mac_prefix;
    memcpy(&(deviceinfo->currentSettings.name[10]), device_mac_array, device_mac_array.length());
    device_mac = QString::fromUtf8(device_mac_array.toHex().toUpper());
    device_mac = QString("%1").arg( device_mac.toLongLong(NULL, 16) , 12, 16, QChar('0')).toUpper();

    QString device_name = ui->LineEditNewName->text();
//    device_name += QString::number( deviceinfo->device_counter );

    int device_channel = ui->BoxNewChannel->currentIndex();
    QString device_channel_str = QString::number(device_channel);

    int name_len = device_name.length();
    QByteArray device_name09 = QByteArray::fromStdString(device_name.toLocal8Bit().constData());
    memset(deviceinfo->currentSettings.name, 0, 10);
    memcpy(deviceinfo->currentSettings.name, device_name09, name_len);

    deviceinfo->currentSettings.channel = device_channel;
    uint16_t device_number_add = ui->LineEditNewNumber->text().toUInt();

    QStringList device_info;
    QList<QStandardItem*> newRow;
    for(int i = 0; i < device_number_add; i++) {
        device_info << device_mac;                          // 0
        device_info << device_name + QString::number(deviceinfo->device_number);    // 1
        device_info << device_channel_str;                  // 2
        device_info << "0";                                 // 3
        for(int j = 0; j < model->columnCount(); j++) {
            QStandardItem* item = new QStandardItem(device_info.at(j));
            newRow.append(item);
        }
        device_mac = QString("%1").arg((device_mac.toLongLong(NULL, 16) + 1), 12, 16, QChar('0')).toUpper();
        device_info.clear();
        model->appendRow(newRow);
        newRow.clear();
        deviceinfo->device_number++;
    }

//    QString new_id = QString("%1").arg( model->data(model->index(model->rowCount()-1, 0)).toString().toULongLong(NULL,16)+1 , 12, 16, QChar('0')).toUpper();
    ui->LineEditNewOrder->setText(QString::number(device_mac_number + device_number_add, 16).toUpper());

    if(ui->ActionTest->isChecked()) {
       return ;
    }

    ActionFileSaveHandler();

    dictionary *ini;
    FILE *fp;
    QString temp;
    QStringList ini_device_status;

    ini = iniparser_load( "archive.ini" );
    temp = "Status:" + device_type_group.at(device_mac_order - 1);
    iniparser_set(ini, temp.toLocal8Bit().data(), ui->LineEditNewOrder->text().toLocal8Bit().constData());
    fp = fopen("archive.ini", "w");
    iniparser_dump_ini(ini, fp);
    iniparser_freedict(ini);
    fclose(fp);
}

void MainWindow::ButtonDeviceDeleteHandler()
{
    return ;
    if((deviceinfo->device_number == 0) || (model->rowCount() == 0) || (db == NULL)) {
        return ;
    }
    QModelIndexList currentIndexList = ui->tableView->selectionModel()->selectedRows();
    if(currentIndexList.isEmpty()) {
        return ;
    }
    model->removeRows(currentIndexList.at(0).row(), currentIndexList.count());

    if(deviceinfo->device_number >= currentIndexList.count()) {
        deviceinfo->device_number -= currentIndexList.count();
    }

    if(model->rowCount() == 0) {
        deviceinfo->device_number = 0;
    }

//    sqlite3_stmt *stmt;
//    sqlite3_prepare_v2(db, "INSERT INTO DEVICE (ID, NAME, CHANNEL) VALUES (?, ?, ?);", 41, &stmt, NULL);
//    if(stmt != NULL) {
//        sqlite3_bind_text(stmt, 1, "John", SQLITE_TRANSIENT);
//        sqlite3_bind_int(stmt, 2, number);
//        sqlite3_step(stmt);
//        sqlite3_finalize(stmt);
//    }
}

void MainWindow::ButtonSetbyHandHandler()
{
    if( (serial == NULL) || !(serial->isOpen() )) {
        QMessageBox::warning(this, tr("自动配置"), tr("串口未打开.\n"
                                                     "请点击打开串口."),QMessageBox::Ok | QMessageBox::Cancel);
        return ;
    }

    if(db == NULL) {
        QMessageBox MessageBoxAddDevice(this);
        MessageBoxAddDevice.setIcon(QMessageBox::Warning);
        MessageBoxAddDevice.setWindowTitle(tr("新增设备"));
        MessageBoxAddDevice.setText(tr("当前未选中数据库文件,请选择你需要执行的操作。\n"
                                       "新建数据库： 新建一个.db结尾的数据库文件并保存到硬盘。\n"
                                       "打开数据库： 从硬盘打开一个以.db结尾的数据库文件，并载入到表格。\n"
                                       " 取  消：       取消新增设备的操作。"));
        QAbstractButton* ButtonNew = MessageBoxAddDevice.addButton(tr("新建数据库"), QMessageBox::NoRole);
        QAbstractButton* ButtonOpen = MessageBoxAddDevice.addButton(tr("打开数据库"), QMessageBox::NoRole);
        QAbstractButton* ButtonCancel = MessageBoxAddDevice.addButton(tr("取消"), QMessageBox::NoRole);
        MessageBoxAddDevice.exec();
        if(MessageBoxAddDevice.clickedButton() == ButtonNew) {
            ActionFileNewHandler();
        } else if(MessageBoxAddDevice.clickedButton() == ButtonOpen) {
            ActionFileOpenHandler();
        } else if(MessageBoxAddDevice.clickedButton() == ButtonCancel) {
            return ;
        }
    } else {

    }

    if(model->rowCount() == 0) {
        QMessageBox::warning(this, tr("手动配置"), tr("表格为空.\n"
                                                    "请先添加设备."), QMessageBox::Ok | QMessageBox::Cancel);
        return ;
    }

    if(deviceinfo->SetByHandFlag == 0) {

        if(ui->tableView->selectionModel()->selectedRows().isEmpty()) {
            QMessageBox::warning(this, tr("手动配置"), tr("当前未选中设备MAC.\n"
                                                         "请先选中一个MAC."), QMessageBox::Ok | QMessageBox::Cancel);
            return ;
        }

        deviceinfo->SetByHandFlag = 1;
        ui->ButtonSetbyHand->setText("手动配置中");
        ui->ButtonSetbyHand->setStyleSheet("background-color: rgb(255,220,220);border: none; ");
        ui->ButtonSetAuto->setEnabled(false);
    } else {
        deviceinfo->SetByHandFlag = 0;
        ui->ButtonSetbyHand->setText("手动配置");
        ui->ButtonSetbyHand->setStyleSheet("");
        ui->ButtonSetAuto->setEnabled(true);
    }
}

void MainWindow::ButtonSetAutoHandler()
{
    if( (serial == NULL) || !(serial->isOpen() )) {
        QMessageBox::warning(this, tr("自动配置"), tr("串口未打开.\n"
                                                     "请点击打开串口."), QMessageBox::Ok | QMessageBox::Cancel);
        return ;
    }

    if(db == NULL) {
        QMessageBox MessageBoxAddDevice(this);
        MessageBoxAddDevice.setIcon(QMessageBox::Warning);
        MessageBoxAddDevice.setWindowTitle(tr("新增设备"));
        MessageBoxAddDevice.setText(tr("当前未选中数据库文件,请选择你需要执行的操作。\n"
                                       "新建数据库： 新建一个.db结尾的数据库文件并保存到硬盘。\n"
                                       "打开数据库： 从硬盘打开一个以.db结尾的数据库文件，并载入到表格。\n"
                                       " 取  消：       取消新增设备的操作。"));
        QAbstractButton* ButtonNew = MessageBoxAddDevice.addButton(tr("新建数据库"), QMessageBox::NoRole);
        QAbstractButton* ButtonOpen = MessageBoxAddDevice.addButton(tr("打开数据库"), QMessageBox::NoRole);
        QAbstractButton* ButtonCancel = MessageBoxAddDevice.addButton(tr("取消"), QMessageBox::NoRole);
        MessageBoxAddDevice.exec();
        if(MessageBoxAddDevice.clickedButton() == ButtonNew) {
            ActionFileNewHandler();
        } else if(MessageBoxAddDevice.clickedButton() == ButtonOpen) {
            ActionFileOpenHandler();
        } else if(MessageBoxAddDevice.clickedButton() == ButtonCancel) {
            return ;
        }
    } else {
        if(model->rowCount() == 0) {
            QMessageBox::warning(this, tr("自动配置"),
                                       tr("表格数据为空.\n"
                                          "请先添加设备."), QMessageBox::Ok | QMessageBox::Cancel);
            return ;
        }

    }

    if(deviceinfo->SetAutoFlag == 0) {
        ui->ButtonSetAuto->setText("停止自动配置");
        ui->ButtonSetAuto->setStyleSheet("background-color: rgb(255,220,220);border: none; ");
        ui->ButtonSetbyHand->setEnabled(false);
        deviceinfo->SetAutoFlag = 1;
    } else {
        ui->ButtonSetAuto->setText("开始自动配置");
        ui->ButtonSetAuto->setStyleSheet("");
        ui->ButtonSetbyHand->setEnabled(true);
        deviceinfo->SetAutoFlag = 0;
    }
}

void MainWindow::ActionFileNewHandler()
{
    QFileDialog dialog(this);
    dialog.setDefaultSuffix("db");
    dialog.setNameFilter(tr("DB Files (*.db)"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return ;

    model->setRowCount(0);

    deviceinfo->device_been_set = 0;
    deviceinfo->device_number = 0;

    deviceinfo->SetByHandFlag = 0;
    ui->ButtonSetbyHand->setStyleSheet("");
    ui->ButtonSetbyHand->setText("手动配置");
    ui->ButtonSetbyHand->setEnabled(true);

    deviceinfo->SetAutoFlag = 0;
    ui->ButtonSetAuto->setStyleSheet("");
    ui->ButtonSetAuto->setText("开始自动配置");
    ui->ButtonSetAuto->setEnabled(true);

    QString fileName = dialog.selectedFiles().first();

    char *zErrMsg = 0;

    if(db != NULL) {
        sqlite3_close(db);
    }
    int rc;
    rc = sqlite3_open(fileName.toLocal8Bit().constData(), &db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    this->setWindowTitle(tr("设备配置") + " - " + fileName);

    const char *sql;
    sql = "CREATE TABLE DEVICE("  \
             "ID             VARCHAR(20)    NOT NULL," \
             "NAME           TEXT    NOT NULL," \
             "CHANNEL        INT     NOT NULL," \
             "STATUS         TEXT    NOT NULL);";

    rc = sqlite3_exec(db, sql, callback_create_insert, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
//        fprintf(stdout, "Table created successfully\n");
    }

    statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::ActionFileOpenHandler()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("DB Files (*.db)"));
    if (fileName.isEmpty() || fileName.isNull()) {
        return ;
    }

    deviceinfo->SetByHandFlag = 0;
    ui->ButtonSetbyHand->setStyleSheet("");
    ui->ButtonSetbyHand->setText("手动配置");
    ui->ButtonSetbyHand->setEnabled(true);

    deviceinfo->SetAutoFlag = 0;
    ui->ButtonSetAuto->setStyleSheet("");
    ui->ButtonSetAuto->setText("开始自动配置");
    ui->ButtonSetAuto->setEnabled(true);

    char *zErrMsg = 0;
    const char *data = NULL;

    model->setRowCount(0);

    if(db != NULL) {
        sqlite3_close(db);
    }

    int rc;
    rc = sqlite3_open(fileName.toLocal8Bit().constData(), &db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    this->setWindowTitle(tr("设备配置") + " - " + fileName);

    const char *sql;
    sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='DEVICE' ;";

    rc = sqlite3_exec(db, sql, callback_check_table_exists, &data, &zErrMsg);
    while( ( data == NULL ) || ( strcmp("DEVICE", data) != 0 )) {  // for number: atoi(data)

        const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Open Database File"),
                                   tr("Database format is not correct!"),
                                   QMessageBox::Open | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Open:
            ActionFileOpenHandler();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }

        break;
    }

    int row = 0;
    const unsigned char* id, *name, *channel, *status;
    sql = "SELECT * FROM DEVICE;";
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        return ;
    bool done = false;
    while (!done) {
        switch (sqlite3_step (stmt)) {
        case SQLITE_ROW:
            id      = sqlite3_column_text(stmt, 0);
            name    = sqlite3_column_text(stmt, 1);
            channel = sqlite3_column_text(stmt, 2);
            status  = sqlite3_column_text(stmt, 3);

            model->setRowCount(row+1);

            model->setData(model->index(row, 0), QString::fromUtf8((const char *)id));
            model->setData(model->index(row, 1), QString::fromUtf8((const char *)name));
            model->setData(model->index(row, 2), QString::fromUtf8((const char *)channel));
            model->setData(model->index(row, 3), QString::fromUtf8((const char *)status));

            row++;
            break;

        case SQLITE_DONE:
            done = true;
            break;

        default:
            fprintf(stderr, "Failed.\n");
            return ;
        }
    }
    sqlite3_finalize(stmt);

    sql = "SELECT COUNT(*) FROM DEVICE where STATUS = '已配置';";
    rc = sqlite3_exec(db, sql, callback_check_table_exists, &data, NULL);
    if (rc != SQLITE_OK)
        return ;

    deviceinfo->device_been_set = QString::fromUtf8(data).toInt(NULL);

    ui->tableView->selectRow( deviceinfo->device_been_set );
    ui->tableView->scrollTo(model->index(deviceinfo->device_been_set + 4, 0));

//    QString new_id = QString("%1").arg( model->data(model->index(model->rowCount()-1, 0)).toString().toULongLong(NULL,16)+1 , 12, 16, QChar('0')).toUpper();
//    ui->LineEditNewOrder->setText(new_id);

    const char *s;
    dictionary *ini;
    ini = iniparser_load( "archive.ini" );
    s = iniparser_getstring(ini, "Status:SettingCounter", NULL);
    QString tmp_str = QString::fromLocal8Bit(s);
//    ui->LineEditNewOrder->setText( tmp_str );
    iniparser_freedict(ini);

#if 0

    sql =   "INSERT INTO DEVICE (ID,NAME,CHANNEL) "  \
            "VALUES (1, 'MasterControl', 2 ); " \
            "INSERT INTO DEVICE (ID,NAME,CHANNEL) "  \
            "VALUES (2, 'PowerControl', 1 ); "     \
            "INSERT INTO DEVICE (ID,NAME,CHANNEL) "  \
            "VALUES (3, 'Security', 1 );" \
            "INSERT INTO DEVICE (ID,NAME,CHANNEL) "  \
            "VALUES (4, 'Window', 4 );";

    rc = sqlite3_exec(db, sql, callback_create_insert, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "Records created successfully\n");
    }
//    rc = RunSqlNoCallback(db, sql);
//    if( rc != SQLITE_OK ){
//        fprintf(stderr, "insert info error\n");
//    } else {
//        fprintf(stdout, "Operation done successfully\n");
//    }

    sql = "SELECT * from TEST";
    rc = sqlite3_exec(db, sql, callback_query_update_delete, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "Operation done successfully\n");
    }
//    rc = RunSqlNoCallback(db, sql);
//    if( rc != SQLITE_OK ){
//        fprintf(stderr, "Query error\n");
//    } else {
//        fprintf(stdout, "Operation done successfully\n");
//    }

    fflush(stdout);
    sqlite3_close(db);
#endif

//    QFile file(fileName);
//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("Application"),
//                             tr("Cannot read file %1:\n%2.")
//                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
//        return;
//    }
}

void MainWindow::ActionFileSaveHandler()
{
    char *zSQL, *sErrMsg = 0;
    QModelIndex index;
    QString sql_id, sql_name, sql_channel, sql_status;
    uint16_t sql_save_counter = 0;

    const char *data = NULL;
    zSQL = "SELECT COUNT(*) FROM DEVICE;";
    sqlite3_exec(db, zSQL, callback_check_table_exists, &data, NULL);
    sql_save_counter = QString::fromUtf8(data).toInt(NULL);

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    for( ; sql_save_counter < model->rowCount(); sql_save_counter++) {
        index = model->index(sql_save_counter, 0, QModelIndex());
        sql_id = ui->tableView->model()->data(index).toString();
        index = model->index(sql_save_counter, 1, QModelIndex());
        sql_name = ui->tableView->model()->data(index).toString();
        index = model->index(sql_save_counter, 2, QModelIndex());
        sql_channel = ui->tableView->model()->data(index).toString();
        index = model->index(sql_save_counter, 3, QModelIndex());
        sql_status = ui->tableView->model()->data(index).toString();

        zSQL = sqlite3_mprintf("INSERT INTO DEVICE (ID, NAME, CHANNEL, STATUS) VALUES ('%q', '%q', '%q', '%q'); ", \
                              sql_id.toUpper().toStdString().c_str(), sql_name.toStdString().c_str(), sql_channel.toStdString().c_str(), sql_status.toStdString().c_str());
        sqlite3_exec(db,zSQL,0,0,NULL);
    }
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    sqlite3_free(zSQL);
}

void MainWindow::ActionFileExitHandler()
{
    const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
//        save();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void MainWindow::ActionFileHelpHandler()
{
    QTextEdit *text = new QTextEdit();
    text->setWindowTitle(tr("帮助/使用说明"));
    text->setFixedHeight(400);
    text->setFixedWidth(800);
    text->setWindowFlags(Qt::Window);
    text->setReadOnly(true);
    text->append("本程序使用说明：\n"
                 "\n"
                 "01. 每次打开程序，需要先新建或者打开一个数据库文件。\n"
                 "\n"
                 "02. 点击菜单栏左上角的 \"文件\",选择\"新建存档\"或者\"打开存档\",新建或者打开一个数据库文件。\n"
                 "\n"
                 "03. 新建或者打开数据库文件后，可以新增设备。\n"
                 "\n"
                 "04. 新增设备的时候，需要依次设置好开始的\"设备ID\",\"名称\",\"数量\",\"类型\",\"路数\",然后点击\"新增设备\"\n"
                 "\n"
                 "05. 此时\"设备属性\"窗口中就能显示出刚才新增的设备。\n"
                 "\n"
                 "06. 然后点击\"刷新串口\",在下拉框中选择对应正确的串口号。\n"
                 "\n"
                 "07. 点击\"开始自动配置\"，此时程序就等待设备的回应，开始自动配置设备。\n"
                 "\n"
                 "08. 当成功配置完一个设备后，当前配置的ID会标记为\"已配置\"，程序会自动移动到下一个ID，继续自动配置。\n"
                 "\n"
                 "09. 系统每配置完一个设备，会自动备份到数据库，关闭软件后可以再次打开此数据库文件，继续上次的自动配置。\n"
                 "\n"
                 "10. 对于测试设备，可以采用手动配置的模式，在表格中，选中需要配置的ID，然后点击手动配置，再插入设备即可。\n"
                 "\n");
    text->show();
}

void MainWindow::ActionFileTestHandler()
{
    static QString string_back;
    static int index_back;
    static int back_current_row;

    ui->ButtonSetAuto->setText("开始自动配置");
    ui->ButtonSetAuto->setStyleSheet("");
    ui->ButtonSetAuto->setEnabled(true);
    deviceinfo->SetAutoFlag = 0;

    ui->ButtonSetbyHand->setText("手动配置");
    ui->ButtonSetbyHand->setStyleSheet("");
    ui->ButtonSetbyHand->setEnabled(true);
    deviceinfo->SetByHandFlag = 0;

    if(ui->ActionTest->isChecked()) {
        string_back = ui->LineEditNewOrder->text();
        index_back = ui->BoxNewType->currentIndex();
        back_current_row = model->rowCount();
        ui->LineEditNewOrder->setEnabled(true);
    } else {
        ui->LineEditNewOrder->setText(string_back);
        ui->BoxNewType->setCurrentIndex(index_back);
        ui->LineEditNewOrder->setEnabled(false);
        model->setRowCount(back_current_row);

        // read the whole db again
        int row = 0,rc = 0;
        const char *sql;
        const char *data = NULL;
        const unsigned char* id, *name, *channel, *status;
        sql = "SELECT * FROM DEVICE;";
        sqlite3_stmt *stmt = NULL;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK)
            return ;
        bool done = false;
        while (!done) {
            switch (sqlite3_step (stmt)) {
            case SQLITE_ROW:
                id      = sqlite3_column_text(stmt, 0);
                name    = sqlite3_column_text(stmt, 1);
                channel = sqlite3_column_text(stmt, 2);
                status  = sqlite3_column_text(stmt, 3);

                model->setRowCount(row+1);

                model->setData(model->index(row, 0), QString::fromUtf8((const char *)id));
                model->setData(model->index(row, 1), QString::fromUtf8((const char *)name));
                model->setData(model->index(row, 2), QString::fromUtf8((const char *)channel));
                model->setData(model->index(row, 3), QString::fromUtf8((const char *)status));

                row++;
                break;

            case SQLITE_DONE:
                done = true;
                break;

            default:
                fprintf(stderr, "Failed.\n");
                return ;
            }
        }
        sqlite3_finalize(stmt);

        sql = "SELECT COUNT(*) FROM DEVICE where STATUS = '已配置';";
        rc = sqlite3_exec(db, sql, callback_check_table_exists, &data, NULL);
        if (rc != SQLITE_OK)
            return ;

        deviceinfo->device_been_set = QString::fromUtf8(data).toInt(NULL);

        ui->tableView->selectRow( deviceinfo->device_been_set );
        ui->tableView->scrollTo(model->index(deviceinfo->device_been_set + 4, 0));
    }
}

void MainWindow::ActionMSMACHandler()
{
    QFileDialog dialog(this);
    dialog.setDefaultSuffix("xls");
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return ;
    QString fileName = dialog.selectedFiles().first();

    FILE *fp;

    fp = fopen(fileName.toLocal8Bit().constData(), "w");
    if(fp == NULL) {
        QMessageBox::warning(this, tr("中控MAC地址生成"), tr("打开文件错误.\n"
                                                           "请选择正确的文件."),
                                           QMessageBox::Ok | QMessageBox::Cancel);
        return ;
    }

    char *base_str = "000000000220";
    unsigned long long base_number = strtol(base_str, NULL, 16);

    for(int i = 0; i < 100; i++) {

        fprintf(fp, "00,01,");
        fprintf(fp, "%012llX", base_number);
        fprintf(fp, ",01,0000\n");

        base_number++;

    }

    fflush(fp);
    fclose(fp);
}

void MainWindow::BoxTypeChangedHandler(int index)
{
    dictionary *ini;
    int device_counter;
    QStringList ini_device_status;
    ini = iniparser_load( "archive.ini" );
    for(int i = 0; i < device_type_group.length(); i++) {
        QString temp = "Status:" + device_type_group.at(i);
        device_counter = iniparser_getint(ini, temp.toLocal8Bit().data(), NULL);
        ini_device_status << QString::fromLocal8Bit(iniparser_getstring(ini, temp.toLocal8Bit().data(), NULL));
    }

    if(index == 0) {
        ui->LineEditNewOrder->setText("0");
    } else {
        ui->LineEditNewOrder->setText(ini_device_status.at(index - 1));
    }

    deviceinfo->device_number = ui->LineEditNewOrder->text().toULong(NULL, 16) - 0x800;
    iniparser_freedict(ini);
}
