#include "customlineitem.h"

CustomLineItem::CustomLineItem(QLineF line, QGraphicsItem *parent):
    QGraphicsLineItem(line, parent), selectionOffset(4)
{
    setFlags(QGraphicsItem::ItemIsSelectable
             |QGraphicsItem::ItemIsMovable);
    createSelectionPolygon();
}

void CustomLineItem::createSelectionPolygon(){
    QPolygonF nPolygon;
    qreal radAngle = line().angle()* M_PI / 180;
    qreal dx = selectionOffset * sin(radAngle);
    qreal dy = selectionOffset * cos(radAngle);
    QPointF offset1 = QPointF(dx, dy);
    QPointF offset2 = QPointF(-dx, -dy);
    nPolygon << line().p1() + offset1
             << line().p1() + offset2
             << line().p2() + offset2
             << line().p2() + offset1;
    selectionPolygon = nPolygon;
    update();
}

QRectF CustomLineItem::boundingRect() const {
    return selectionPolygon.boundingRect();
}

QPainterPath CustomLineItem::shape() const{
    QPainterPath ret;
    ret.addPolygon(selectionPolygon);
    return ret;
}

void CustomLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->drawLine(line());
    if (isSelected()) {
        painter->setPen(QPen(Qt::black, 2, Qt::DashLine));
        painter->drawPolygon(selectionPolygon);
    }
}
