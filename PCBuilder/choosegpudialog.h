#ifndef CHOOSEGPUDIALOG_H
#define CHOOSEGPUDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseGPUDialog;
}

class ChooseGPUDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseGPUDialog(QWidget *parent = nullptr);
    ~ChooseGPUDialog();

    void initializeModel(int id);

signals:
    void sendGPUID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChooseGPUDialog *ui;

    bool createConnection();
};

#endif // CHOOSEGPUDIALOG_H
