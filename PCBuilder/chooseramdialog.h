#ifndef CHOOSERAMDIALOG_H
#define CHOOSERAMDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseRAMDialog;
}

class ChooseRAMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseRAMDialog(QWidget *parent = nullptr);
    ~ChooseRAMDialog();

    void initializeModel(int motherboard_id, int cpu_id);

signals:
    void sendRAMID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChooseRAMDialog *ui;

    bool createConnection();
};

#endif // CHOOSERAMDIALOG_H
