#ifndef CHOOSEMOTHERBOARDDIALOG_H
#define CHOOSEMOTHERBOARDDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseMotherboardDialog;
}

class QSqlQueryModel;

class ChooseMotherboardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseMotherboardDialog(QWidget *parent = nullptr);
    ~ChooseMotherboardDialog();

signals:
    void sendMotherboardID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChooseMotherboardDialog *ui;

    bool createConnection();
    void initializeModel(QSqlQueryModel *model);
};

#endif // CHOOSEMOTHERBOARDDIALOG_H
