#ifndef CUSTOMRECTITEM_H
#define CUSTOMRECTITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

class CustomRectItem : public QGraphicsRectItem
{
public:
    CustomRectItem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CUSTOMRECTITEM_H
