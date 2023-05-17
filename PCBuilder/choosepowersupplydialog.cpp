#include "choosepowersupplydialog.h"
#include "ui_choosepowersupplydialog.h"
#include <QtSql>
#include <QtWidgets>

ChoosePowerSupplyDialog::ChoosePowerSupplyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoosePowerSupplyDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChoosePowerSupplyDialog::rowDoubleClicked);
}

ChoosePowerSupplyDialog::~ChoosePowerSupplyDialog()
{
    delete ui;
}

void ChoosePowerSupplyDialog::initializeModel(int gpu_id)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    ui->tableView->setModel(model);

    if (gpu_id != NULL) {
        model->setQuery("SELECT * FROM power_supply WHERE max_power > "
                        "(SELECT max_power_consumption FROM gpu WHERE id = "
                        + QString::number(gpu_id) + ")");
    } else {
        model->setQuery("SELECT * FROM power_supply WHERE max_power > 500");
    }

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Brand"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Series"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Model"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Maximum Power"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Price"));

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void ChoosePowerSupplyDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendPowerSupplyID(id);
    this->close();
}

bool ChoosePowerSupplyDialog::createConnection()
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
    query.exec("CREATE TABLE power_supply ("
               "id INTEGER PRIMARY KEY, "
               "brand TEXT, "
               "series TEXT, "
               "model TEXT, "
               "max_power INTEGER, "
               "price REAL)"
               );
    query.exec("INSERT INTO power_supply (id, brand, series, model, max_power, price) "
               "VALUES (1, 'Corsair', 'RMx', 'RM850x', 850, 129.99), "
               "(2, 'EVGA', 'SuperNOVA', '750 G5', 750, 119.99), "
               "(3, 'Seasonic', 'Focus', 'Focus GX-650', 650, 109.99), "
               "(4, 'Cooler Master', 'MWE', 'MWE Gold 750 V2', 750, 99.99), "
               "(5, 'Thermaltake', 'Toughpower', 'Toughpower Grand RGB 850W', 850, 139.99)");

    return true;
}

void ChoosePowerSupplyDialog::on_btnCancel_clicked()
{
    reject();
}

void ChoosePowerSupplyDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendPowerSupplyID(id);
    }
    this->close();
}

