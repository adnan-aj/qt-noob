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

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPointF m_lastMousePos;
    bool isPanning;
    int panStartX, panStartY;
};

#endif // CHARTVIEWZOOM_H
