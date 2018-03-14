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
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    Qt::KeyboardModifiers _modifiers;
    bool isPanning;
    int panStartX, panStartY;
    qreal scaleFactorX=1.0;

};

#endif // CHARTVIEWZOOM_H
