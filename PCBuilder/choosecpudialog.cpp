#include "choosecpudialog.h"
#include "ui_choosecpudialog.h"
#include <QtSql>
#include <QtWidgets>

ChooseCPUDialog::ChooseCPUDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseCPUDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);


    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChooseCPUDialog::rowDoubleClicked);
}

ChooseCPUDialog::~ChooseCPUDialog()
{
    delete ui;
}

void ChooseCPUDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendCPUID(id);
    this->close();
}

bool ChooseCPUDialog::createConnection()
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
    query.exec("CREATE TABLE cpu ("
               "id INT PRIMARY KEY, "
               "brand TEXT, "
               "name TEXT, "
               "cpu_socket_type TEXT, "
               "cores INT, "
               "threads INT, "
               "integrated_graphics TEXT, "
               "mem_type TEXT, "
               "mem_speed INT, "
               "mem_slot_count INT, "
               "max_mem_supported INT, "
               "price REAL)");
    query.exec("INSERT INTO cpu VALUES ("
               "1, 'Intel', 'Core i5-11600K', 'LGA 1200', 6, 12, 'Intel UHD Graphics 750', 'DDR4', 3200, 2, 128, 269.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "2, 'AMD', 'Ryzen 7 5800X', 'AM4', 8, 16, 'N/A', 'DDR4', 3200, 2, 128, 429.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "3, 'Intel', 'Core i9-11900K', 'LGA 1200', 8, 16, 'Intel UHD Graphics 750', 'DDR4', 3200, 2, 128, 589.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "4, 'AMD', 'Ryzen 5 5600X', 'AM4', 6, 12, 'N/A', 'DDR4', 3200, 2, 128, 299.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "5, 'Intel', 'Core i7-11700K', 'LGA 1200', 8, 16, 'Intel UHD Graphics 750', 'DDR4', 3200, 2, 128, 399.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "6, 'AMD', 'Ryzen 9 5900X', 'AM4', 12, 24, 'N/A', 'DDR4', 3200, 2, 128, 549.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "7, 'Intel', 'Core i5-11600', 'LGA 1200', 6, 12, 'Intel UHD Graphics 750', 'DDR4', 3200, 2, 128, 219.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "8, 'AMD', 'Ryzen 7 5700X', 'AM4', 8, 16, 'N/A', 'DDR4', 3200, 2, 128, 379.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "9, 'Intel', 'Core i9-11900', 'LGA 1200', 8, 16, 'Intel UHD Graphics 750', 'DDR4', 3200, 2, 128, 509.99)");
    query.exec("INSERT INTO cpu VALUES ("
               "10, 'AMD', 'Ryzen 5 5600', 'AM4', 6, 12, 'N/A', 'DDR4', 3200, 2, 128, 229.99)");

    return true;
}

void ChooseCPUDialog::initializeModel(int id)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    ui->tableView->setModel(model);

    model->setQuery("SELECT cpu.* "
                    "FROM cpu "
                    "INNER JOIN motherboard ON cpu.cpu_socket_type = motherboard.cpu_socket_type "
                    "WHERE motherboard.id = " + QString::number(id));

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Brand"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CPU Socket Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Cores"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Threads"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Integrated Graphics"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Memory Standard"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Memory Speed"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Number of Memory Slots"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Maximum Memory Supported"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("Price"));

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void ChooseCPUDialog::on_btnCancel_clicked()
{
    reject();
}

void ChooseCPUDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendCPUID(id);
    }
    this->close();
}

