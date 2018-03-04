#ifndef GRAPHICSVIEWZOOM_H
#define GRAPHICSVIEWZOOM_H

#include <QObject>
#include <QGraphicsView>
#include <QTime>

class GraphicsViewZoom : public QGraphicsView
{
    /* https://stackoverflow.com/questions/6650219/zooming-function-on-a-qwidget */
    /* https://stackoverflow.com/questions/4753681/how-to-pan-images-in-qgraphicsview */
    /* http://www.walletfox.com/course/qgraphicsitemsnaptogrid.php */

    Q_OBJECT

public:
    GraphicsViewZoom(QWidget *parent = Q_NULLPTR);
    void gentle_zoom(double factor);
    void set_zoom_factor_base(double value);
    int getGridSize() const { return this->gridSize; }

protected:
    //void drawBackground(QPainter* painter, const QRectF &rect);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Qt::KeyboardModifiers _modifiers;
    double _zoom_factor_base;
    QPointF target_scene_pos, target_viewport_pos;
    bool eventFilter(QObject* object, QEvent* event);
    bool isPanning;
    int _panStartX, _panStartY;
    int gridSize;
    QTime buttonPressTime;

signals:
    void zoomed();
    void tLog(QString);
};

#endif // GRAPHICSVIEWZOOM_H
