#include "choosestoragedialog.h"
#include "ui_choosestoragedialog.h"
#include <QtSql>
#include <QtWidgets>

ChooseStorageDialog::ChooseStorageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseStorageDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChooseStorageDialog::rowDoubleClicked);
}

ChooseStorageDialog::~ChooseStorageDialog()
{
    qDebug() << "ChooseStorageDialog::~ChooseStorageDialog()";
    model->setQuery("DELETE FROM storage");

    delete ui;
}

void ChooseStorageDialog::initializeModel()
{
    model = new QSqlQueryModel;

    ui->tableView->setModel(model);

    //model->setQuery("DELETE FROM storage");
    model->setQuery("SELECT * FROM storage");

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Interface"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Capacity"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Price"));

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void ChooseStorageDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendStorageID(id);
    this->close();
}

bool ChooseStorageDialog::createConnection()
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
    query.exec("CREATE TABLE storage ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT, "
               "type TEXT, "
               "interface TEXT, "
               "capacity INTEGER, "
               "price REAL)");

    query.exec("INSERT INTO storage (name, type, interface, capacity, price) "
               "VALUES ('Seagate Barracuda', 'HDD', 'SATA', 2000, 59.99)");
    query.exec("INSERT INTO storage (name, type, interface, capacity, price) "
               "VALUES ('Samsung 860 EVO', 'SSD', 'SATA', 500, 99.99)");
    query.exec("INSERT INTO storage (name, type, interface, capacity, price) "
               "VALUES ('WD Blue', 'HDD', 'SATA', 1000, 49.99)");
    query.exec("INSERT INTO storage (name, type, interface, capacity, price) "
               "VALUES ('Crucial MX500', 'SSD', 'SATA', 1000, 119.99)");

    return true;
}

void ChooseStorageDialog::on_btnCancel_clicked()
{
    reject();
}

void ChooseStorageDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendStorageID(id);
    }
    this->close();
}

