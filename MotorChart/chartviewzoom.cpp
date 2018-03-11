#include "chartviewzoom.h"
#include <QApplication>
#include <QDebug>

ChartViewZoom::ChartViewZoom(QWidget *parent) : QChartView(parent)
{
    //setDragMode(QGraphicsView::NoDrag);
    this->setMouseTracking(true);
    isPanning = false;
}

void ChartViewZoom::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        //QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
        m_lastMousePos = event->pos();
        panStartX = event->x();
        panStartY = event->y();
        isPanning = true;
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QChartView::mousePressEvent(event);
}
void ChartViewZoom::mouseReleaseEvent(QMouseEvent *event)
{
    isPanning = false;
    if (event->button() == Qt::MiddleButton)
    {
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    QChartView::mousePressEvent(event);
}

void ChartViewZoom::mouseMoveEvent(QMouseEvent *event)
{
#if 0
    // pan the chart with a middle mouse drag
    if (event->buttons() & Qt::MiddleButton)
    {
        QRectF bounds = QRectF(0,0,0,0);
//        for(auto series : this->chart()->series())
//            bounds.united(series->bounds());
        bounds = this->chart()->boundingRect();

        auto dPos = this->chart()->mapToValue(event->pos()) - this->chart()->mapToValue(m_lastMousePos);

        if (this->rubberBand() == QChartView::RectangleRubberBand)
            this->chart()->zoomIn(bounds.translated(-dPos.x(), -dPos.y()));
        else if (this->rubberBand() == QChartView::HorizontalRubberBand)
            this->chart()->zoomIn(bounds.translated(-dPos.x(), 0));
        else if (this->rubberBand() == QChartView::VerticalRubberBand)
            this->chart()->zoomIn(bounds.translated(0, -dPos.y()));

        m_lastMousePos = event->pos();
        event->accept();
    }
#endif
    if (isPanning) {
               //chart()->scroll(-10, 0);
        this->chart()->scroll((qreal)(panStartX - event->x()) / 10.0, 0);
        event->accept();
        return;
    }
    QChartView::mouseMoveEvent(event);
}
