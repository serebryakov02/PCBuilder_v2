#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->setFixedSize(488, 282);

    on_checkBox_stateChanged(0);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_checkBox_stateChanged(int arg1)
{
    if (!arg1) {
        ui->widget->hide();
    }
    else {
        ui->widget->show();
    }
}

void Dialog::resizeEvent(QResizeEvent */*event*/)
{
    qDebug() << "width: " << width() << ", height: " << height();
}


void Dialog::on_btnExit_clicked()
{
    reject();
}


void Dialog::on_btnCreateNew_clicked()
{
    accept();
}

