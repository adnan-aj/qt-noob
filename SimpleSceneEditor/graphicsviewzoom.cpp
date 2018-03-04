#include "graphicsviewzoom.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>
#include <QTime>

#include <mainwindow.h>

GraphicsViewZoom::GraphicsViewZoom(QWidget *parent)
    : QGraphicsView(parent), gridSize(20)
{
  this->viewport()->installEventFilter(this);
  this->setMouseTracking(true);
  _modifiers = Qt::ControlModifier;
  _zoom_factor_base = 1.0015;
  isPanning = false;
}

void GraphicsViewZoom::gentle_zoom(double factor) {
  this->scale(factor, factor);
  this->centerOn(target_scene_pos);
  QPointF delta_viewport_pos = target_viewport_pos - QPointF(this->viewport()->width() / 2.0,
                                                             this->viewport()->height() / 2.0);
  QPointF viewport_center = this->mapFromScene(target_scene_pos) - delta_viewport_pos;
  this->centerOn(this->mapToScene(viewport_center.toPoint()));
  emit zoomed();
}

void GraphicsViewZoom::set_zoom_factor_base(double value) {
  _zoom_factor_base = value;
}

bool GraphicsViewZoom::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::MouseMove) {
    QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
    QPointF delta = target_viewport_pos - mouse_event->pos();
    if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
      target_viewport_pos = mouse_event->pos();
      target_scene_pos = this->mapToScene(mouse_event->pos());
    }
  } else if (event->type() == QEvent::Wheel) {
    QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
    if (QApplication::keyboardModifiers() == _modifiers) {
      if (wheel_event->orientation() == Qt::Vertical) {
        double angle = wheel_event->angleDelta().y();
        double factor = qPow(_zoom_factor_base, angle);
        gentle_zoom(factor);
        return true;
      }
    }
  }
  Q_UNUSED(object)
  return false;
}

void GraphicsViewZoom::mousePressEvent(QMouseEvent *event)
{
    buttonPressTime.restart();
    if (event->button() == Qt::RightButton)
    {
        isPanning = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
    //event->ignore();
}

void GraphicsViewZoom::mouseReleaseEvent(QMouseEvent *event)
{
    int t = buttonPressTime.elapsed();
    emit tLog(tr("Mouse depress time = ") + QString::number(t) + tr(" msecs"));

    if (event->button() == Qt::RightButton)
    {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
    //event->ignore();
}

void GraphicsViewZoom::mouseMoveEvent(QMouseEvent *event)
{
    if (isPanning)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
    //event->ignore();
}

void GraphicsViewZoom::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPen pen;
    pen.setWidth(1);
    painter->setPen(pen);

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    emit tLog(tr("ViewZoom::drawBackground ") + QString::number(int(rect.width() / gridSize)) + "," + QString::number(int(rect.height() / gridSize)));

    QVector<QPointF> points;
    for (qreal x = left; x < rect.right(); x += gridSize){
        for (qreal y = top; y < rect.bottom(); y += gridSize){
            points.append(QPointF(x,y));
        }
    }
    painter->drawPoints(points.data(), points.size());
}
