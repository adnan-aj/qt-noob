#ifndef CUSTOMLINEITEM_H
#define CUSTOMLINEITEM_H

#include <QGraphicsLineItem>
#include <QPen>
#include <QPainter>
#include <cmath>

using namespace std;

class CustomLineItem : public QGraphicsLineItem
{
public:
    CustomLineItem(QLineF line, QGraphicsItem* parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option,
                QWidget* widget = 0);
private:
    const qreal selectionOffset;
    QPolygonF selectionPolygon;
    void createSelectionPolygon();
};

#endif // CUSTOMLINEITEM_H
