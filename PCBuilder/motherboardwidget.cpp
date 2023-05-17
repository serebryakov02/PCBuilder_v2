#include "motherboardwidget.h"
#include "item.h"
#include <QMouseEvent>

MotherboardWidget::MotherboardWidget(QWidget *parent)
    : QGraphicsView{parent},
    scene(new QGraphicsScene)
{
    QPixmap pixmap(":/images/mb.png");
    scene->addPixmap(pixmap);

    // CPU
    scene->addItem(new Item(QRectF(QPointF(196.852, 127.538), QPointF(263.469, 192.231))));
    // RAM1
    scene->addItem(new Item(QRectF(QPointF(334.632, 85.9495), QPointF(344.722, 269.863))));
    // RAM2
    scene->addItem(new Item(QRectF(QPointF(376.22, 85.0253), QPointF(385.786, 270.787))));
    // GPU
    scene->addItem(new Item(QRectF(QPointF(124.765, 321.617), QPointF(310.527, 330.859))));
    // PCI1
    scene->addItem(new Item(QRectF(QPointF(154.339, 377.069), QPointF(341.025, 387.235))));
    // PCI2
    scene->addItem(new Item(QRectF(QPointF(153.339, 418.657), QPointF(341.101, 427.899))));

    this->setScene(scene);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setDragMode(QGraphicsView::RubberBandDrag);

    connect(this, &MotherboardWidget::rubberBandChanged, this,
            [](QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint){
                qDebug() << "from: " << fromScenePoint
                         << "\tto: " << toScenePoint;
            });
}

void MotherboardWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    this->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MotherboardWidget::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePos = this->mapToScene(event->pos());

    if (QRectF(QPointF(196.852, 127.538), QPointF(263.469, 192.231)).contains(scenePos)) {
        emit cpuSocketClicked();
        qDebug() << "CPU Socket clicked";
    }
    if (QRectF(QPointF(334.632, 85.9495), QPointF(344.722, 269.863)).contains(scenePos)) {
        emit ramSlot1Clicked();
        qDebug() << "RAM1 Slot clicked";
    }
    if (QRectF(QPointF(376.22, 85.0253), QPointF(385.786, 270.787)).contains(scenePos)) {
        emit ramSlot2Clicked();
        qDebug() << "RAM2 Slot clicked";
    }
    if (QRectF(QPointF(124.765, 321.617), QPointF(310.527, 330.859)).contains(scenePos)) {
        emit gpuSlotClicked();
        qDebug() << "GPU Slot clicked";
    }
    if (QRectF(QPointF(154.339, 377.069), QPointF(341.025, 387.235)).contains(scenePos)) {
        emit pciSlot1Clicked();
        qDebug() << "PCI1 Slot clicked";
    }
    if (QRectF(QPointF(153.339, 418.657), QPointF(341.101, 427.899)).contains(scenePos)) {
        emit pciSlot2Clicked();
        qDebug() << "PCI2 Slot clicked";
    }
}
