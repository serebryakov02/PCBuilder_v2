#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_btnExit_clicked();

    void on_btnCreateNew_clicked();

private:
    Ui::Dialog *ui;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};
#endif // DIALOG_H
