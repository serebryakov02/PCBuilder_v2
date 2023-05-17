#include "item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

Item::Item(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setAcceptHoverEvents(true);
    path.addRoundedRect(rect, 5, 5);
    color = Qt::transparent;
}

QRectF Item::boundingRect() const
{
    return path.boundingRect();
}

void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/,
                 QWidget */*widget*/)
{
    painter->fillPath(path, color);
}

void Item::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsObject::hoverEnterEvent(event);
    color = Qt::yellow;
    qDebug() << "Item::hoverEnterEvent";
}

void Item::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsObject::hoverLeaveEvent(event);
    color = Qt::transparent;
    qDebug() << "Item::hoverLeaveEvent";
}

//void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if (QRectF(QPointF(196.852, 127.538), QPointF(263.469, 192.231)).contains(event->pos())) {
//        emit cpuSocketClicked();
//        qDebug() << "CPU Socket clicked";
//    }
//    if (QRectF(QPointF(334.632, 85.9495), QPointF(344.722, 269.863)).contains(event->pos())) {
//        emit ramSlot1Clicked();
//        qDebug() << "RAM1 Slot clicked";
//    }
//    if (QRectF(QPointF(376.22, 85.0253), QPointF(385.786, 270.787)).contains(event->pos())) {
//        emit ramSlot2Clicked();
//        qDebug() << "RAM2 Slot clicked";
//    }
//    if (QRectF(QPointF(124.765, 321.617), QPointF(310.527, 330.859)).contains(event->pos())) {
//        emit gpuSlotClicked();
//        qDebug() << "GPU Slot clicked";
//    }
//    if (QRectF(QPointF(154.339, 377.069), QPointF(341.025, 387.235)).contains(event->pos())) {
//        emit pciSlot1Clicked();
//        qDebug() << "PCI1 Slot clicked";
//    }
//    if (QRectF(QPointF(153.339, 418.657), QPointF(341.101, 427.899)).contains(event->pos())) {
//        emit pciSlot2Clicked();
//        qDebug() << "PCI2 Slot clicked";
//    }
//}
