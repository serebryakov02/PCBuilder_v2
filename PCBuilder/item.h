#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsObject>

class Item : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Item(const QRectF &rect, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

//signals:
//    void cpuSocketClicked();
//    void ramSlot1Clicked();
//    void ramSlot2Clicked();
//    void gpuSlotClicked();
//    void pciSlot1Clicked();
//    void pciSlot2Clicked();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPainterPath path;
    QColor color;
};

#endif // ITEM_H
