#include "chooseramdialog.h"
#include "ui_chooseramdialog.h"
#include <QtSql>
#include <QtWidgets>

ChooseRAMDialog::ChooseRAMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseRAMDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChooseRAMDialog::rowDoubleClicked);
}

ChooseRAMDialog::~ChooseRAMDialog()
{
    delete ui;
}

void ChooseRAMDialog::initializeModel(int motherboard_id, int cpu_id)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    ui->tableView->setModel(model);

    model->setQuery("SELECT * FROM ram "
                    "WHERE mem_type = (SELECT mem_type FROM motherboard WHERE id = "
                    + QString::number(motherboard_id) + ") "
                                                        "AND mem_speed <= (SELECT mem_speed FROM motherboard WHERE id = "
                    + QString::number(motherboard_id) + ") "
                                                        "AND capacity <= (SELECT max_mem_supported FROM motherboard WHERE id = "
                    + QString::number(motherboard_id) + ") "
                                                        "AND quantity <= (SELECT mem_slot_count FROM motherboard WHERE id = "
                    + QString::number(motherboard_id) + ") "
                                                        "AND mem_speed <= (SELECT mem_speed FROM cpu WHERE id = "
                    + QString::number(cpu_id) + ") "
                                                "AND capacity <= (SELECT max_mem_supported FROM cpu WHERE id = "
                    + QString::number(cpu_id) + ") "
                                                "AND quantity <= (SELECT mem_slot_count FROM cpu WHERE id = "
                    + QString::number(cpu_id) + ") "
                                                "AND mem_type = (SELECT mem_type FROM cpu WHERE id = "
                    + QString::number(cpu_id) + ")");



    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Brand"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Series"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacity"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantity"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Memory Type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Memory Speed"));

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void ChooseRAMDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendRAMID(id);
    this->close();
}

bool ChooseRAMDialog::createConnection()
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
    query.exec("CREATE TABLE ram ("
               "id INT PRIMARY KEY, "
               "brand TEXT, "
               "series TEXT, "
               "capacity INT, "
               "quantity INT, "
               "mem_type TEXT, "
               "mem_speed INT, "
               "price REAL)");
    query.exec("INSERT INTO ram VALUES ("
               "1, 'Corsair', 'Vengeance LPX', 16, 2, 'DDR4', 2400, 79.99)");
    query.exec("INSERT INTO ram VALUES ("
               "2, 'Kingston', 'HyperX Fury', 8, 2, 'DDR4', 3200, 49.99)");
    query.exec("INSERT INTO ram VALUES ("
               "3, 'G.Skill', 'Ripjaws V', 32, 2, 'DDR4', 3600, 189.99)");
    query.exec("INSERT INTO ram VALUES ("
               "4, 'Crucial', 'Ballistix', 32, 2, 'DDR4', 3200, 149.99)");
    query.exec("INSERT INTO ram VALUES ("
               "5, 'Team Group', 'T-Force Vulcan Z', 16, 2, 'DDR4', 2400, 69.99)");
    query.exec("INSERT INTO ram VALUES ("
               "6, 'ADATA', 'XPG Spectrix D50', 16, 1, 'DDR4', 3200, 84.99)");
    query.exec("INSERT INTO ram VALUES ("
               "7, 'Patriot', 'Viper 4 Blackout', 16, 1, 'DDR4', 2400, 79.99)");
    query.exec("INSERT INTO ram VALUES ("
               "8, 'Team T-Force', 'Dark Za', 16, 1, 'DDR4', 3600, 89.99)");
    query.exec("INSERT INTO ram VALUES ("
               "9, 'HyperX', 'Predator', 32, 1, 'DDR4', 3200, 169.99)");
    query.exec("INSERT INTO ram VALUES ("
               "10, 'Corsair', 'Vengeance RGB Pro', 16, 1, 'DDR4', 3200, 94.99)");
    query.exec("INSERT INTO ram VALUES ("
               "11, 'ADATA', 'XPG Z1', 128, 2, 'DDR4', 2400, 369.99)");
    query.exec("INSERT INTO ram VALUES ("
               "12, 'Patriot', 'Viper 4 Blackout', 128, 2, 'DDR4', 3200, 419.99)");

    return true;
}

void ChooseRAMDialog::on_btnCancel_clicked()
{
    reject();
}

void ChooseRAMDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendRAMID(id);
    }
    this->close();
}

