#include "choosenetworkadapterdialog.h"
#include "ui_choosenetworkadapterdialog.h"
#include <QtSql>
#include <QtWidgets>

ChooseNetworkAdapterDialog::ChooseNetworkAdapterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseNetworkAdapterDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChooseNetworkAdapterDialog::rowDoubleClicked);
}

ChooseNetworkAdapterDialog::~ChooseNetworkAdapterDialog()
{
    delete ui;
}

void ChooseNetworkAdapterDialog::initializeModel(int motherboard_id)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    ui->tableView->setModel(model);

    model->setQuery("SELECT * FROM network_adapter WHERE interface = "
                    "(SELECT interface FROM motherboard WHERE id = "
                    + QString::number(motherboard_id) + ")");

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Brand"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Model"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Speed"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Interface"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Price"));

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void ChooseNetworkAdapterDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendNetworkAdapterID(id);
    this->close();
}

bool ChooseNetworkAdapterDialog::createConnection()
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
    query.exec("CREATE TABLE network_adapter ("
               "id INTEGER PRIMARY KEY, "
               "brand TEXT, "
               "model TEXT, "
               "speed INTEGER, "
               "interface TEXT, "
               "price REAL)");

    query.exec("INSERT INTO network_adapter (brand, model, speed, interface, price) VALUES"
               "(1, 'Intel', 'Dual Band Wireless-AC 8265', 867, 'Wi-Fi', 24.99),"
               "(2, 'TP-Link', 'Archer T3U Plus', 1300, 'Wi-Fi', 29.99),"
               "(3, 'Asus', 'USB-AC68', 1900, 'Wi-Fi', 79.99),"
               "(4, 'Netgear', 'Nighthawk A7000', 1900, 'USB', 99.99),"
               "(5, 'Linksys', 'WUSB6300', 867, 'USB', 49.99),"
               "(6, 'Trendnet', 'TEW-809UB', 1900, 'USB', 69.99),"
               "(7, 'D-Link', 'DWA-192', 1900, 'USB', 79.99),"
               "(8, 'Belkin', 'F9L1109', 867, 'USB', 34.99),"
               "(9, 'Apple', 'Thunderbolt to Gigabit Ethernet Adapter', 1000, 'Thunderbolt', 29.00),"
               "(10, 'Amazon Basics', 'USB 3.0 to Ethernet Adapter', 1000, 'USB', 14.99)");

    query.exec("UPDATE network_adapter SET interface = 'PCIe 3.0 x16' WHERE id IN (2, 4, 6, 8, 10)");
    query.exec("UPDATE network_adapter SET interface = 'PCIe 4.0 x16' WHERE id IN (1, 3, 5, 7, 9)");

    return true;
}

void ChooseNetworkAdapterDialog::on_btnCancel_clicked()
{
    reject();
}


void ChooseNetworkAdapterDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendNetworkAdapterID(id);
    }
    this->close();
}

