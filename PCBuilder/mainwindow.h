#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectMotherboardBtnClicked();
    void CPUslotClicked();
    void RAM1slotClicked();
    void RAM2slotClicked();
    void GPUslotClicked();
    void selectStorageBtnClicked();
    void PCI1slotClicked();
    void PCI2slotClicked();
    void selectPowerSupplyBtnClicked();
    void selectCaseBtnClicked();
    void testSlot();

private:
    Ui::MainWindow *ui;

    QLabel *lblMotherboard;
    int currentMBid;
    QLabel *lblCPU;
    int currentCPUid;
    QLabel *lblRAM1;
    int currentRAM1id;
    QLabel *lblRAM2;
    int currentRAM2id;
    QLabel *lblGPU;
    int currentGPUid;
    QLabel *lblStorage;
    int currentStorageId;
    QString storageInfo;
    double storage_price;
    QLabel *lblPCI1;
    int currentPCI1id;
    QLabel *lblPCI2;
    int currentPCI2id;
    QLabel *lblPowerSupply;
    int currentPowerSupplyId;
    QLabel* lblCase;
    QString caseInfo;
    double case_price;
    int currentCaseId;

    void init_gui();

    int pciMaxValue;
    int pciAmountUsed;
};
#endif // MAINWINDOW_H
