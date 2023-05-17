#ifndef CHOOSEPOWERSUPPLYDIALOG_H
#define CHOOSEPOWERSUPPLYDIALOG_H

#include <QDialog>

namespace Ui {
class ChoosePowerSupplyDialog;
}

class ChoosePowerSupplyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoosePowerSupplyDialog(QWidget *parent = nullptr);
    ~ChoosePowerSupplyDialog();

    void initializeModel(int gpu_id = NULL);

signals:
    void sendPowerSupplyID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChoosePowerSupplyDialog *ui;

    bool createConnection();
};

#endif // CHOOSEPOWERSUPPLYDIALOG_H
