#ifndef CHOOSENETWORKADAPTERDIALOG_H
#define CHOOSENETWORKADAPTERDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseNetworkAdapterDialog;
}

class ChooseNetworkAdapterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseNetworkAdapterDialog(QWidget *parent = nullptr);
    ~ChooseNetworkAdapterDialog();

    void initializeModel(int motherboard_id);

signals:
    void sendNetworkAdapterID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChooseNetworkAdapterDialog *ui;

    bool createConnection();
};

#endif // CHOOSENETWORKADAPTERDIALOG_H
