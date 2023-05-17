#include "choosemotherboarddialog.h"
#include "ui_choosemotherboarddialog.h"
#include <QtSql>
#include <QtWidgets>

ChooseMotherboardDialog::ChooseMotherboardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseMotherboardDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);

    QSqlQueryModel* model = new QSqlQueryModel;
    initializeModel(model);

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChooseMotherboardDialog::rowDoubleClicked);
}

ChooseMotherboardDialog::~ChooseMotherboardDialog()
{
    delete ui;
}

void ChooseMotherboardDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendMotherboardID(id);
    this->close();
}

bool ChooseMotherboardDialog::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection.\n\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;
    query.exec("DROP TABLE motherboard");
    query.exec("CREATE TABLE motherboard ("
               "id INT PRIMARY KEY, "
               "brand TEXT, "
               "model TEXT, "
               "cpu_socket_type TEXT, "
               "chipset TEXT,"
               "mem_type TEXT, "
               "mem_speed INT, "
               "mem_slot_count INT, "
               "max_mem_supported INT, "
               "interface TEXT, "
               "interface_count INT, "
               "form_factor TEXT, "
               "price REAL)");
    query.exec("INSERT INTO motherboard VALUES ("
               "1, 'ASRock', 'H610M-HDV', 'LGA 1700', 'Intel H610', 'DDR4', 3200, 2, 64, 'PCIe 3.0 x16', 1, 'Micro-ATX', 79.99)");
    query.exec("INSERT INTO motherboard VALUES ("
               "2, 'Gigabyte', 'B450M DS3H', 'AM4', 'AMD B450', 'DDR4', 3200, 2, 64, 'PCIe 3.0 x16', 1, 'Micro-ATX', 74.99)");
    query.exec("INSERT INTO motherboard VALUES ("
               "3, 'ASUS', 'ROG Strix B560-F Gaming WIFI', 'LGA 1200', 'Intel B560', 'DDR4', 3200, 2, 128, 'PCIe 4.0 x16', 3, 'ATX', 229.99)");
    query.exec("INSERT INTO motherboard VALUES ("
               "4, 'MSI', 'A520M-A PRO', 'AM4', 'AMD A520', 'DDR4', 3200, 2, 64, 'PCIe 3.0 x16', 1, 'Micro-ATX', 69.99)");
    query.exec("INSERT INTO motherboard VALUES ("
               "5, 'ASRock', 'Z590 Extreme WIFI 6E', 'LGA 1200', 'Intel Z590', 'DDR4', 5600, 2, 128, 'PCIe 4.0 x16', 3, 'ATX', 529.99)");
    query.exec("INSERT INTO motherboard VALUES ("
               "6, 'Gigabyte', 'B460M D3H', 'LGA 1200', 'Intel B460', 'DDR4', 2400, 2, 64, 'PCIe 3.0 x16', 2, 'Micro-ATX', 99.99)");

    return true;
}

void ChooseMotherboardDialog::initializeModel(QSqlQueryModel *model)
{
    model->setQuery("SELECT * FROM motherboard");

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Brand"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Model"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CPU Socket Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Chipset"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Memory Standard"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Memory Speed"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Number of Memory Slots"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Maximum Memory Supported"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Interface"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Interface Count"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("Form Factor"));
    model->setHeaderData(12, Qt::Horizontal, QObject::tr("Price"));
}

void ChooseMotherboardDialog::on_btnCancel_clicked()
{
    reject();
}

void ChooseMotherboardDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendMotherboardID(id);
    }
    this->close();
}

