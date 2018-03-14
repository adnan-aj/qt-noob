#ifndef CHARTVIEWZOOM_H
#define CHARTVIEWZOOM_H

#include <QObject>
#include <QtCharts/QChartView>
QT_CHARTS_USE_NAMESPACE

class ChartViewZoom : public QChartView
{
    Q_OBJECT

public:
    ChartViewZoom(QWidget *parent = Q_NULLPTR);

private:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    QPointF m_lastMousePos;
    bool isPanning;
    int panStartX, panStartY;
};

#endif // CHARTVIEWZOOM_H
