#ifndef CHOOSESOUNDCARDDIALOG_H
#define CHOOSESOUNDCARDDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseSoundCardDialog;
}

class ChooseSoundCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseSoundCardDialog(QWidget *parent = nullptr);
    ~ChooseSoundCardDialog();

    void initializeModel(int motherboard_id);

signals:
    void sendSoundCardID(int id);

private slots:
    void rowDoubleClicked(const QModelIndex &index);

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::ChooseSoundCardDialog *ui;

    bool createConnection();
};

#endif // CHOOSESOUNDCARDDIALOG_H
