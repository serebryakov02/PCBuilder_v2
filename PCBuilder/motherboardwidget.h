#ifndef MOTHERBOARDWIDGET_H
#define MOTHERBOARDWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>

class MotherboardWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MotherboardWidget(QWidget *parent = nullptr);

signals:
    void cpuSocketClicked();
    void ramSlot1Clicked();
    void ramSlot2Clicked();
    void gpuSlotClicked();
    void pciSlot1Clicked();
    void pciSlot2Clicked();

private:
    QGraphicsScene *scene;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // MOTHERBOARDWIDGET_H
