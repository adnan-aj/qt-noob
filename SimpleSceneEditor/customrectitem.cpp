#include "customrectitem.h"
#include <QDebug>

CustomRectItem::CustomRectItem()
{
    pressed = false;
    setFlag(ItemIsMovable);
    this->setRect(0, 0, 100, 100);
}

QRectF CustomRectItem::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void CustomRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);

    if (pressed) {
        brush.setColor(Qt::green);
    }
    else if (isSelected()) {
        brush.setColor(Qt::magenta);
    }
    else {
        brush.setColor(Qt::yellow);
    }

    painter->fillRect(rec, brush);
}

void CustomRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CustomRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsItem::mousePressEvent(event);
}
