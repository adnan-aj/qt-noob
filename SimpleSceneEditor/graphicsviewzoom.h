#ifndef GRAPHICSVIEWZOOM_H
#define GRAPHICSVIEWZOOM_H

#include <QObject>
#include <QGraphicsView>
#include <QTime>

class GraphicsViewZoom : public QGraphicsView
{
    /* https://stackoverflow.com/questions/6650219/zooming-function-on-a-qwidget
 */
    Q_OBJECT

public:
    enum Mode {NoMode, SelectObject, DrawLine};
    GraphicsViewZoom(QWidget *parent = Q_NULLPTR);
    void gentle_zoom(double factor);
    void set_zoom_factor_base(double value);

    /* http://www.walletfox.com/course/qgraphicsitemsnaptogrid.php */
    int getGridSize() const { return this->gridSize; }

protected:
    /* http://www.walletfox.com/course/qgraphicsitemsnaptogrid.php */
    void drawBackground(QPainter* painter, const QRectF &rect);

private:
    QGraphicsView* _view;
    Mode sceneMode;
    Qt::KeyboardModifiers _modifiers;
    double _zoom_factor_base;
    QPointF target_scene_pos, target_viewport_pos;
    bool eventFilter(QObject* object, QEvent* event);
    QPointF origPoint;
    QGraphicsLineItem* itemToDraw;
    bool isPanning;
    bool isZooming;
    bool isPlacing;
    bool isDragging;

    /* https://stackoverflow.com/questions/4753681/how-to-pan-images-in-qgraphicsview */
    QTime buttonPressTime;
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    //bool _pan;
    int _panStartX, _panStartY;

    /* http://www.walletfox.com/course/qgraphicsitemsnaptogrid.php */
    int gridSize;

signals:
    void zoomed();
    void tLog(QString);
};

#endif // GRAPHICSVIEWZOOM_H
