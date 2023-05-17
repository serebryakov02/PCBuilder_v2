#ifndef CHOOSECASEDIALOG_H
#define CHOOSECASEDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseCaseDialog;
}

class ChooseCaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseCaseDialog(QWidget *parent = nullptr);
    ~ChooseCaseDialog();

    void initializeModel(int motherboard_id);

signals:
    void sendCaseID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChooseCaseDialog *ui;

    bool createConnection();
};

#endif // CHOOSECASEDIALOG_H
