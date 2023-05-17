#include "choosegpudialog.h"
#include "ui_choosegpudialog.h"
#include <QtSql>
#include <QtWidgets>

ChooseGPUDialog::ChooseGPUDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseGPUDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChooseGPUDialog::rowDoubleClicked);
}

ChooseGPUDialog::~ChooseGPUDialog()
{
    delete ui;
}

void ChooseGPUDialog::initializeModel(int id)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    ui->tableView->setModel(model);

    model->setQuery("SELECT gpu.* "
                    "FROM gpu "
                    "INNER JOIN motherboard ON gpu.interface = motherboard.interface "
                    "WHERE motherboard.id = " + QString::number(id));

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Model"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Interface"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Memory Size"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Memory Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Max Power Consumption"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Price"));

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void ChooseGPUDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendGPUID(id);
    this->close();
}

bool ChooseGPUDialog::createConnection()
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
    query.exec("DROP TABLE gpu");
    query.exec("CREATE TABLE gpu ("
               "id INT PRIMARY KEY, "
               "model TEXT, "
               "interface TEXT, "
               "mem_size INT, "
               "mem_type TEXT, "
               "max_power_consumption INT, "
               "price REAL)");
    query.exec("INSERT INTO gpu (id, model, interface, mem_size, mem_type, max_power_consumption, price) "
               "VALUES (1, 'NVIDIA GeForce RTX 3080', 'PCIe 4.0 x16', 10, 'GDDR6X', 320, 999.99), "
               "(2, 'AMD Radeon RX 6800 XT', 'PCIe 4.0 x16', 16, 'GDDR6', 300, 899.99), "
               "(3, 'NVIDIA GeForce GTX 1660 Super', 'PCIe 3.0 x16', 6, 'GDDR6', 125, 299.99), "
               "(4, 'AMD Radeon RX 5700 XT', 'PCIe 3.0 x16', 8, 'GDDR6', 180, 449.99)");

    return true;
}

void ChooseGPUDialog::on_btnCancel_clicked()
{
    reject();
}

void ChooseGPUDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendGPUID(id);
    }
    this->close();
}

