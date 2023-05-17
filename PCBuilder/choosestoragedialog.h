#ifndef CHOOSESTORAGEDIALOG_H
#define CHOOSESTORAGEDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseStorageDialog;
}

class QSqlQueryModel;

class ChooseStorageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseStorageDialog(QWidget *parent = nullptr);
    ~ChooseStorageDialog();

    void initializeModel();

signals:
    void sendStorageID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChooseStorageDialog *ui;

    QSqlQueryModel* model;
    bool createConnection();
};

#endif // CHOOSESTORAGEDIALOG_H
