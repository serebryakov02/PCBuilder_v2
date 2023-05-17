#include "choosecasedialog.h"
#include "ui_choosecasedialog.h"
#include <QtSql>
#include <QtWidgets>

ChooseCaseDialog::ChooseCaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseCaseDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChooseCaseDialog::rowDoubleClicked);
}

ChooseCaseDialog::~ChooseCaseDialog()
{
    delete ui;
}

void ChooseCaseDialog::initializeModel(int motherboard_id)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    ui->tableView->setModel(model);

    model->setQuery("SELECT * FROM pc_case WHERE form_factor = "
                    "(SELECT form_factor FROM motherboard WHERE id = "
                    + QString::number(motherboard_id) + ")");
//    model->setQuery("SELECT * FROM pc_case");


    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Brand"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Series"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Model"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Form Factor"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Price"));

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void ChooseCaseDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendCaseID(id);
    this->close();
}

bool ChooseCaseDialog::createConnection()
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
    query.exec("CREATE TABLE pc_case ("
               "id INTEGER PRIMARY KEY, "
               "brand TEXT, "
               "series TEXT, "
               "model TEXT, "
               "form_factor TEXT, "
               "price REAL)"
               );
    query.exec("INSERT INTO pc_case (id, brand, series, model, form_factor, price) "
               "VALUES (1, 'NZXT', 'H', 'H510', 'Mid Tower', 79.99), "
               "(2, 'Fractal Design', 'Meshify', 'Meshify C', 'Mid Tower', 89.99), "
               "(3, 'Corsair', 'Obsidian', 'Obsidian 500D RGB SE', 'Full Tower', 249.99), "
               "(4, 'Phanteks', 'Eclipse', 'Eclipse P400S', 'Mid Tower', 79.99), "
               "(5, 'Cooler Master', 'MasterBox', 'MasterBox Q300L', 'Micro-ATX', 49.99)");

    return true;
}

void ChooseCaseDialog::on_btnCancel_clicked()
{
    reject();
}

void ChooseCaseDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendCaseID(id);
    }
    this->close();
}

