#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "motherboardwidget.h"
#include "choosemotherboarddialog.h"
#include "choosecpudialog.h"
#include "chooseramdialog.h"
#include "choosegpudialog.h"
#include "choosestoragedialog.h"
#include "choosenetworkadapterdialog.h"
#include "choosesoundcarddialog.h"
#include "choosepowersupplydialog.h"
#include "choosecasedialog.h"
#include <QtWidgets>
#include <QtSql>
#include <QtPrintSupport>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    storage_price(0.0),
    case_price(0.0),
    pciAmountUsed(0)
{
    ui->setupUi(this);

    auto dialog = new Dialog(this);
    if (dialog->exec() != QDialog::Rejected)
    {
        init_gui();
    } else {
        exit(EXIT_SUCCESS);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectMotherboardBtnClicked()
{
    auto dialog = new ChooseMotherboardDialog(this);
    connect(dialog, &ChooseMotherboardDialog::sendMotherboardID, this,
            [this](int id){
                currentMBid = id;
                QSqlQuery query;
                query.exec("SELECT brand, model FROM motherboard WHERE id = " + QString::number(id));

                QString motherboardInfo;
                if (query.next()) {
                    motherboardInfo += query.value(0).toString();
                    motherboardInfo += " ";
                    motherboardInfo += query.value(1).toString();
                }

                this->lblMotherboard->setText("<span style='background: green'>" +
                                              motherboardInfo + "</span>");
            });
    dialog->exec();
}

void MainWindow::CPUslotClicked()
{
    auto dialog = new ChooseCPUDialog(this);
    dialog->initializeModel(currentMBid);
    connect(dialog, &ChooseCPUDialog::sendCPUID, this,
            [this](int id){
                currentCPUid = id;
                QSqlQuery query;
                query.exec("SELECT brand, name FROM cpu WHERE id = " + QString::number(id));

                QString CPUInfo;
                if (query.next()) {
                    CPUInfo += query.value(0).toString();
                    CPUInfo += " ";
                    CPUInfo += query.value(1).toString();
                }

                this->lblCPU->setText("<span style='background: green'>" +
                                      CPUInfo + "</span>");

                query.exec("SELECT integrated_graphics FROM cpu WHERE "
                           "id = " + QString::number(id));

                QString integrated_graphics;
                if (query.next()) {
                    integrated_graphics = query.value(0).toString();

                    if (integrated_graphics != "N/A")
                        this->lblGPU->setText("<span style='background: green'>" +
                                              integrated_graphics + "</span>");
                }
            });
    dialog->exec();
}

void MainWindow::RAM1slotClicked()
{
    auto dialog = new ChooseRAMDialog(this);
    dialog->initializeModel(currentMBid, currentCPUid);
    connect(dialog, &ChooseRAMDialog::sendRAMID, this,
            [this](int id){
                currentRAM1id = id;
                QSqlQuery query;
                query.exec("SELECT quantity FROM ram WHERE id = " + QString::number(id));

                int quantity = 0;
                if (query.next()) {
                    quantity = query.value(0).toInt();
                    qDebug() << "quantity:\t" << quantity;
                }

                query.exec("SELECT brand, series, capacity, mem_speed"
                           " FROM ram WHERE id = " + QString::number(id));

                QString RAMInfo;
                if (query.next()) {
                    RAMInfo += query.value(0).toString();
                    RAMInfo += " ";
                    RAMInfo += query.value(1).toString();
                    RAMInfo += " ";
                    RAMInfo += query.value(2).toString();
                    RAMInfo += "GB ";
                    RAMInfo += query.value(3).toString();
                }

                if (quantity == 2) {
                    this->lblRAM1->setText("<span style='background: green'>" +
                                           RAMInfo + "</span>");
                    this->lblRAM2->setText("<span style='background: green'>" +
                                           RAMInfo + "</span>");
                }
                else {
                    this->lblRAM1->setText("<span style='background: green'>" +
                                           RAMInfo + "</span>");
                }
            });
    dialog->exec();
}

void MainWindow::RAM2slotClicked()
{
    auto dialog = new ChooseRAMDialog(this);
    dialog->initializeModel(currentMBid, currentCPUid);
    connect(dialog, &ChooseRAMDialog::sendRAMID, this,
            [this](int id){
                currentRAM2id = id;
                QSqlQuery query;
                query.exec("SELECT quantity FROM ram WHERE id = " + QString::number(id));

                int quantity = 0;
                if (query.next()) {
                    quantity = query.value(0).toInt();
                    qDebug() << "quantity:\t" << quantity;
                }

                query.exec("SELECT brand, series, capacity, mem_speed"
                           " FROM ram WHERE id = " + QString::number(id));

                QString RAMInfo;
                if (query.next()) {
                    RAMInfo += query.value(0).toString();
                    RAMInfo += " ";
                    RAMInfo += query.value(1).toString();
                    RAMInfo += " ";
                    RAMInfo += query.value(2).toString();
                    RAMInfo += "GB ";
                    RAMInfo += query.value(3).toString();
                }

                if (quantity == 2) {
                    this->lblRAM2->setText("<span style='background: green'>" +
                                           RAMInfo + "</span>");
                    this->lblRAM1->setText("<span style='background: green'>" +
                                           RAMInfo + "</span>");
                }
                else {
                    this->lblRAM2->setText("<span style='background: green'>" +
                                           RAMInfo + "</span>");
                }
            });
    dialog->exec();
}

void MainWindow::GPUslotClicked()
{
    QSqlQuery query;
    query.exec("SELECT interface_count FROM motherboard WHERE id = "
               + QString::number(currentMBid));

    if (query.next()) {
        pciMaxValue = query.value(0).toInt();
        qDebug() << "pciMaxValue: " << pciMaxValue;
    }

    auto dialog = new ChooseGPUDialog(this);
    dialog->initializeModel(currentMBid);
    connect(dialog, &ChooseGPUDialog::sendGPUID, this,
            [this](int id){
                currentGPUid = id;

                QSqlQuery query;
                query.exec("SELECT model, mem_size, mem_type"
                           " FROM gpu WHERE id = " + QString::number(id));

                QString GPUInfo;
                if (query.next()) {
                    GPUInfo += query.value(0).toString();
                    GPUInfo += " ";
                    GPUInfo += query.value(1).toString();
                    GPUInfo += "GB ";
                    GPUInfo += query.value(2).toString();
                }

                this->lblGPU->setText("<span style='background: green'>" +
                                      GPUInfo + "</span>");
            });
    dialog->exec();
}

void MainWindow::selectStorageBtnClicked()
{
    auto dialog = new ChooseStorageDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->initializeModel();
    connect(dialog, &ChooseStorageDialog::sendStorageID, this,
            [this](int id){
                currentStorageId = id;
                QSqlQuery query;
                query.exec("SELECT name, capacity, price "
                           "FROM storage WHERE id = " + QString::number(id));

                if (query.next()) {
                    storageInfo += query.value(0).toString();
                    storageInfo += " ";
                    storageInfo += query.value(1).toString();
                    storageInfo += "GB";
                    storage_price = query.value(2).toDouble();
                }

                this->lblStorage->setText("<span style='background: green'>" +
                                          storageInfo + "</span>");
            });
    dialog->exec();
}

void MainWindow::PCI1slotClicked()
{
    QSqlQuery query;
    query.exec("SELECT interface_count FROM motherboard WHERE id = "
               + QString::number(currentMBid));

    if (query.next()) {
        pciMaxValue = query.value(0).toInt();
        qDebug() << "pciMaxValue: " << pciMaxValue;
    }

    auto dialog = new ChooseNetworkAdapterDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->initializeModel(currentMBid);
    connect(dialog, &ChooseNetworkAdapterDialog::sendNetworkAdapterID, this,
            [this](int id){
                currentPCI1id = id;
                QSqlQuery query;
                query.exec("SELECT brand, model "
                           "FROM network_adapter WHERE id = " + QString::number(id));

                QString netAdapInfo;
                if (query.next()) {
                    netAdapInfo += query.value(0).toString();
                    netAdapInfo += " ";
                    netAdapInfo += query.value(1).toString();
                }

                this->lblPCI1->setText("<span style='background: green'>" +
                                       netAdapInfo + "</span>");
            });
    dialog->exec();
}

void MainWindow::PCI2slotClicked()
{
    auto dialog = new ChooseSoundCardDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->initializeModel(currentMBid);
    connect(dialog, &ChooseSoundCardDialog::sendSoundCardID, this,
            [this](int id){
                currentPCI2id = id;
                QSqlQuery query;
                query.exec("SELECT brand, model "
                           "FROM sound_card WHERE id = " + QString::number(id));

                QString soundCardInfo;
                if (query.next()) {
                    soundCardInfo += query.value(0).toString();
                    soundCardInfo += " ";
                    soundCardInfo += query.value(1).toString();
                }

                this->lblPCI2->setText("<span style='background: green'>" +
                                       soundCardInfo + "</span>");
            });
    dialog->exec();
}

void MainWindow::selectPowerSupplyBtnClicked()
{
    auto dialog = new ChoosePowerSupplyDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->initializeModel(currentGPUid);
    connect(dialog, &ChoosePowerSupplyDialog::sendPowerSupplyID, this,
            [this](int id) {
                currentPowerSupplyId = id;
                QSqlQuery query;
                query.exec("SELECT brand, series "
                           "FROM power_supply WHERE id = " + QString::number(id));

                QString powerSupplyInfo;
                if (query.next()) {
                    powerSupplyInfo += query.value(0).toString();
                    powerSupplyInfo += " ";
                    powerSupplyInfo += query.value(1).toString();
                }

                this->lblPowerSupply->setText("<span style='background: green'>" +
                                              powerSupplyInfo + "</span>");
            });
    dialog->exec();
}

void MainWindow::selectCaseBtnClicked()
{
    auto dialog = new ChooseCaseDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->initializeModel(currentMBid);
    connect(dialog, &ChooseCaseDialog::sendCaseID, this,
            [this](int id){
                currentCaseId = id;
                QSqlQuery query;
                query.exec("SELECT brand, series, form_factor, price "
                           "FROM pc_case WHERE id = " + QString::number(id));

                if (query.next()) {
                    caseInfo += query.value(0).toString();
                    caseInfo += " ";
                    caseInfo += query.value(1).toString();
                    caseInfo += " ";
                    caseInfo += query.value(2).toString();

                    case_price = query.value(3).toDouble();
                    qDebug() << "case_price: " << case_price;
                }

                this->lblCase->setText("<span style='background: green'>" +
                                       caseInfo + "</span>");
            });
    dialog->exec();
}

void MainWindow::testSlot()
{
    QString fileName = "output.pdf";

    // Create a QPdfWriter object specifying the file to write
    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageOrientation(QPageLayout::Portrait);
    writer.setResolution(300);

    // Create a QPainter object to draw on the page.
    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set font and text size
    QFont font("Arial", 12);
    painter.setFont(font);

    // Set position for text output
    int x = 100;
    int y = 100;

    // Write information from variables to a PDF file
    QSqlQuery query;
    double mb_price = 0.0;
    double cpu_price = 0.0;
    double ram1_price = 0.0;
    double ram2_price = 0.0;
    double gpu_price = 0.0;
    double pci1_price = 0.0;
    double pci2_price = 0.0;
    double powerSupply_price = 0.0;
    double total_price = 0.0;

    query.exec("SELECT brand, model, price FROM motherboard WHERE id = "
               + QString::number(currentMBid));
    QString MBinfo;
    if (query.next()) {
        MBinfo += query.value(0).toString() + " " + query.value(1).toString();
        mb_price = query.value(2).toDouble();
        total_price += query.value(2).toDouble();
    }

    query.exec("SELECT brand, name, price FROM cpu WHERE id = "
               + QString::number(currentCPUid));
    QString CPUinfo;
    if (query.next()) {
        CPUinfo += query.value(0).toString() + " " + query.value(1).toString();
        cpu_price = query.value(2).toDouble();;
        total_price += query.value(2).toDouble();
    }

    query.exec("SELECT brand, series, capacity, mem_speed, price"
               " FROM ram WHERE id = " + QString::number(currentRAM1id));
    QString RAM1Info;
    if (query.next()) {
        RAM1Info += query.value(0).toString() + " "
                    + query.value(1).toString() +  " "
                    + query.value(2).toString() + "GB "
                    + query.value(3).toString();
        ram1_price = query.value(4).toDouble();
        total_price += query.value(4).toDouble();
    }

    query.exec("SELECT brand, series, capacity, mem_speed, price"
               " FROM ram WHERE id = " + QString::number(currentRAM2id));
    QString RAM2Info;
    if (query.next()) {
        RAM2Info += query.value(0).toString() + " "
                    + query.value(1).toString() +  " "
                    + query.value(2).toString() + "GB "
                    + query.value(3).toString();
        ram2_price = query.value(4).toDouble();
        total_price += query.value(4).toDouble();
    }

    query.exec("SELECT model, mem_size, mem_type, price"
               " FROM gpu WHERE id = " + QString::number(currentGPUid));
    QString GPUinfo;
    if (query.next()) {
        GPUinfo += query.value(0).toString()+ " " + query.value(1).toString()
                   + "GB " + query.value(2).toString();
        gpu_price = query.value(3).toDouble();
        total_price += query.value(3).toDouble();
    }

    //    query.exec("SELECT name, capacity "
    //               "FROM storage WHERE id = " + QString::number(currentStorageId));
    //    QString storageInfo;
    //    if (query.next()) {
    //        storageInfo += query.value(0).toString();
    //        storageInfo += " ";
    //        storageInfo += query.value(1).toString();
    //        storageInfo += "GB";
    //    }

    query.exec("SELECT brand, model, price "
               "FROM network_adapter WHERE id = " + QString::number(currentPCI1id));
    QString netAdapInfo;
    if (query.next()) {
        netAdapInfo += query.value(0).toString()
                       + " " + query.value(1).toString();
        pci1_price = query.value(2).toDouble();
        total_price += query.value(2).toDouble();
    }

    query.exec("SELECT brand, model, price "
               "FROM sound_card WHERE id = " + QString::number(currentPCI2id));
    QString soundCardInfo;
    if (query.next()) {
        soundCardInfo += query.value(0).toString()
                         + " " + query.value(1).toString();
        pci2_price = query.value(2).toDouble();
        total_price += query.value(2).toDouble();
    }

    query.exec("SELECT brand, series, price "
               "FROM power_supply WHERE id = " + QString::number(currentPowerSupplyId));
    QString powerSupplyInfo;
    if (query.next()) {
        powerSupplyInfo += query.value(0).toString()
                           + " " + query.value(1).toString();
        powerSupply_price = query.value(2).toDouble();
        total_price += query.value(2).toDouble();
    }

    //    query.exec("SELECT brand, series, form_factor, price "
    //               "FROM pc_case WHERE id = " + QString::number(currentCaseId));
    //    QString caseInfo;
    //    if (query.next()) {
    //        caseInfo += query.value(0).toString() + " "
    //                    + query.value(1).toString() + " " + query.value(2).toString();
    //        case_price = query.value(2).toDouble();
    //        total_price += query.value(2).toDouble();
    //    }

    painter.drawText(x, y, "Components");
    x += 500;
    painter.drawText(x, y, "Model");
    x += 1200;
    painter.drawText(x, y, "Price");
    x -= 1700;
    y += 30;
    painter.drawLine(x, y, x + 2000, y);
    y += 60;

    painter.drawText(x, y, "Motherboard: ");
    x += 500;
    painter.drawText(x, y, MBinfo);
    x += 1200;
    painter.drawText(x, y, QString::number(mb_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "CPU: ");
    x += 500;
    painter.drawText(x, y, CPUinfo);
    x += 1200;
    painter.drawText(x, y, QString::number(cpu_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "RAM1: ");
    x += 500;
    painter.drawText(x, y, RAM1Info);
    x += 1200;
    painter.drawText(x, y, QString::number(ram1_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "RAM2: ");
    x += 500;
    painter.drawText(x, y, RAM2Info);
    x += 1200;
    painter.drawText(x, y, QString::number(ram2_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "GPU: ");
    x += 500;
    painter.drawText(x, y, GPUinfo);
    x += 1200;
    painter.drawText(x, y, QString::number(gpu_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "Storage: ");
    x += 500;
    painter.drawText(x, y, storageInfo);
    x += 1200;
    painter.drawText(x, y, QString::number(storage_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "Network Adapter: ");
    x += 500;
    painter.drawText(x, y, netAdapInfo);
    x += 1200;
    painter.drawText(x, y, QString::number(pci1_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "Sound Card: ");
    x += 500;
    painter.drawText(x, y, soundCardInfo);
    x += 1200;
    painter.drawText(x, y, QString::number(pci2_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "Power Supply: ");
    x += 500;
    painter.drawText(x, y, powerSupplyInfo);
    x += 1200;
    painter.drawText(x, y, QString::number(powerSupply_price));
    y += 60;
    x -= 1700;

    painter.drawText(x, y, "Case: ");
    x += 500;
    painter.drawText(x, y, caseInfo);
    x += 1200;
    painter.drawText(x, y, QString::number(case_price));
    y += 60;
    x -= 1700;

    total_price += storage_price += case_price;

    painter.drawText(x, y, QString("Total Price: "));
    x += 1700;
    painter.drawText(x, y, QString::number(total_price));

    x -= 1700;
    y += 30;
    painter.drawLine(x, y, x + 2000, y);
    y += 30;

    painter.drawText(x, y, writer.width(), writer.height(), Qt::AlignHCenter, QString("File was generated at ")
                                                                                  + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));

    // Finish drawing and save the document
    painter.end();

    QString currentPath = QDir::currentPath();
    qDebug() << "Current working directory:" << currentPath;
    // Open .pdf
    QDesktopServices::openUrl(QUrl::fromLocalFile(currentPath + "/output.pdf"));
}

void MainWindow::init_gui()
{
    auto motherboardWidget = new MotherboardWidget;
    ui->layout->addWidget(motherboardWidget);

    auto lbl1 = new QLabel("Motherboard: ");
    lblMotherboard = new QLabel("<span style='background: red'>Not Selected</span>");
    auto selectMotherboardBtn = new QPushButton("Select");
    connect(selectMotherboardBtn, &QPushButton::clicked,
            this, &MainWindow::selectMotherboardBtnClicked);

    auto lbl2 = new QLabel("CPU: ");
    lblCPU = new QLabel("<span style='background: red'>Not Selected</span>");
    connect(motherboardWidget, &MotherboardWidget::cpuSocketClicked, this,
            &MainWindow::CPUslotClicked);

    auto lbl3 = new QLabel("RAM1: ");
    lblRAM1 = new QLabel("<span style='background: red'>Not Selected</span>");
    connect(motherboardWidget, &MotherboardWidget::ramSlot1Clicked,
            this, &MainWindow::RAM1slotClicked);

    auto lbl4 = new QLabel("RAM2: ");
    lblRAM2 = new QLabel("<span style='background: red'>Not Selected</span>");
    connect(motherboardWidget, &MotherboardWidget::ramSlot2Clicked,
            this, &MainWindow::RAM2slotClicked);

    auto lbl5 = new QLabel("GPU: ");
    lblGPU = new QLabel("<span style='background: red'>Not Selected</span>");
    connect(motherboardWidget, &MotherboardWidget::gpuSlotClicked,
            this, &MainWindow::GPUslotClicked);

    auto lbl6 = new QLabel("Storage: ");
    lblStorage = new QLabel("<span style='background: red'>Not Selected</span>");
    auto selectStorageBtn = new QPushButton("Select");
    connect(selectStorageBtn, &QPushButton::clicked,
            this, &MainWindow::selectStorageBtnClicked);

    auto lbl7 = new QLabel("PCI1: ");
    lblPCI1 = new QLabel("<span style='background: red'>Not Selected</span>");
    connect(motherboardWidget, &MotherboardWidget::pciSlot1Clicked,
            this, &MainWindow::PCI1slotClicked);

    auto lbl8 = new QLabel("PCI2: ");
    lblPCI2 = new QLabel("<span style='background: red'>Not Selected</span>");
    connect(motherboardWidget, &MotherboardWidget::pciSlot2Clicked,
            this, &MainWindow::PCI2slotClicked);

    auto lbl9 = new QLabel("Power Supply: ");
    lblPowerSupply = new QLabel("<span style='background: red'>Not Selected</span>");
    auto selectPowerSupplyBtn = new QPushButton("Select");
    connect(selectPowerSupplyBtn, &QPushButton::clicked,
            this, &MainWindow::selectPowerSupplyBtnClicked);

    auto lbl10 = new QLabel("Case: ");
    lblCase = new QLabel("<span style='background: red'>Not Selected</span>");
    auto selectCaseBtn = new QPushButton("Select");
    connect(selectCaseBtn, &QPushButton::clicked,
            this, &MainWindow::selectCaseBtnClicked);

    auto gridLayout = new QGridLayout;
    gridLayout->addWidget(lbl1, 0, 0);
    gridLayout->addWidget(lblMotherboard, 0, 1);
    gridLayout->addWidget(selectMotherboardBtn, 0, 2);
    gridLayout->addWidget(lbl2, 1, 0);
    gridLayout->addWidget(lblCPU, 1, 1);
    gridLayout->addWidget(lbl3, 2, 0);
    gridLayout->addWidget(lblRAM1, 2, 1);
    gridLayout->addWidget(lbl4, 3, 0);
    gridLayout->addWidget(lblRAM2, 3, 1);
    gridLayout->addWidget(lbl5, 4, 0);
    gridLayout->addWidget(lblGPU, 4, 1);
    gridLayout->addWidget(lbl6, 5, 0);
    gridLayout->addWidget(lblStorage, 5, 1);
    gridLayout->addWidget(selectStorageBtn, 5, 2);
    gridLayout->addWidget(lbl7, 6, 0);
    gridLayout->addWidget(lblPCI1, 6, 1);
    gridLayout->addWidget(lbl8, 7, 0);
    gridLayout->addWidget(lblPCI2, 7, 1);
    gridLayout->addWidget(lbl9, 8, 0);
    gridLayout->addWidget(lblPowerSupply, 8, 1);
    gridLayout->addWidget(selectPowerSupplyBtn, 8, 2);
    gridLayout->addWidget(lbl10, 9, 0);
    gridLayout->addWidget(lblCase, 9, 1);
    gridLayout->addWidget(selectCaseBtn, 9, 2);

    auto vLayout = new QVBoxLayout;
    vLayout->addLayout(gridLayout);

    auto vSplitter = new QSplitter(Qt::Vertical);
    vLayout->addWidget(vSplitter);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                          | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &MainWindow::testSlot);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &MainWindow::close);
    vLayout->addWidget(buttonBox);

    ui->groupBox->setLayout(vLayout);

    resize(1024, 768);
}
