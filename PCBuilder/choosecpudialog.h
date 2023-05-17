#ifndef CHOOSECPUDIALOG_H
#define CHOOSECPUDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseCPUDialog;
}

class QSqlQueryModel;

class ChooseCPUDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseCPUDialog(QWidget *parent = nullptr);
    ~ChooseCPUDialog();

    void initializeModel(int id);

signals:
    void sendCPUID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChooseCPUDialog *ui;

    bool createConnection();
};

#endif // CHOOSECPUDIALOG_H
