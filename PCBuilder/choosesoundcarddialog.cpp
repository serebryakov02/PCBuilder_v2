#include "choosesoundcarddialog.h"
#include "ui_choosesoundcarddialog.h"
#include <QtSql>
#include <QtWidgets>

ChooseSoundCardDialog::ChooseSoundCardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseSoundCardDialog)
{
    ui->setupUi(this);

    if (!createConnection())
        exit(EXIT_FAILURE);

    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    resize(1024, 768);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ChooseSoundCardDialog::rowDoubleClicked);
}

ChooseSoundCardDialog::~ChooseSoundCardDialog()
{
    delete ui;
}

void ChooseSoundCardDialog::initializeModel(int motherboard_id)
{
    QSqlQueryModel* model = new QSqlQueryModel;

    ui->tableView->setModel(model);

    model->setQuery("SELECT * FROM sound_card WHERE interface = "
                    "(SELECT interface FROM motherboard WHERE id = "
                    + QString::number(motherboard_id) + ")");
//    model->setQuery("SELECT * FROM sound_card");

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Brand"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Model"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Interface"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Channels"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Price"));

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void ChooseSoundCardDialog::rowDoubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel* model = index.model();
    QVariant value = model->data(model->index(index.row(), 0));
    int id = value.toInt();
    emit sendSoundCardID(id);
    this->close();
}

bool ChooseSoundCardDialog::createConnection()
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
    query.exec("CREATE TABLE sound_card ("
               "id INTEGER PRIMARY KEY, "
               "brand TEXT, "
               "model TEXT, "
               "interface TEXT, "
               "channels INTEGER, "
               "price REAL)");

    query.exec("INSERT INTO sound_card (id, brand, model, interface, channels, price) VALUES"
               "(1, 'Creative', 'Sound Blaster Z', 'PCIe 3.0 x16', 5, 89.99),"
               "(2, 'ASUS', 'Xonar DGX', 'PCIe 3.0 x16', 5, 39.99),"
               "(3, 'EVGA', 'NU Audio Pro 7.1', 'PCIe 4.0 x16', 7, 199.99),"
               "(4, 'HT Omega', 'eClaro 7.1', 'PCIe 4.0 x16', 7, 219.99),"
               "(5, 'Auzentech', 'X-Fi Prelude 7.1', 'PCIe 3.0 x16', 7, 159.99),"
               "(6, 'Sennheiser', 'GSX 1200 PRO', 'USB', 7, 219.99),"
               "(7, 'SteelSeries', 'GameDAC', 'USB', 2, 129.99),"
               "(8, 'Behringer', 'U-Phoria UM2', 'USB', 2, 39.99),"
               "(9, 'Focusrite', 'Scarlett 2i2', 'USB', 2, 169.99),"
               "(10, 'M-Audio', 'M-Track Solo', 'USB', 2, 49.00)");

    return true;
}

void ChooseSoundCardDialog::on_btnCancel_clicked()
{
    reject();
}

void ChooseSoundCardDialog::on_btnOK_clicked()
{
    auto index = ui->tableView->currentIndex();
    if (index.isValid()) {
        const QAbstractItemModel* model = index.model();
        QVariant value = model->data(model->index(index.row(), 0));
        int id = value.toInt();
        emit sendSoundCardID(id);
    }
    this->close();
}

