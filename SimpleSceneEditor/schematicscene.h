#ifndef SCHEMATICSHEET_H
#define SCHEMATICSHEET_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QList>
#include <QMenu>

class SchematicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    SchematicScene(QObject* parent = 0);
    enum Mode {NoMode, SelectObject, AddDot, AddLine, AddRect};
    void setSceneMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void drawBackground(QPainter* painter, const QRectF &rect);

public slots:
    void customMenuRequested(QPoint pos);


private slots:
    void action1();
    void action2();
    void action3();

signals:
    void statusEvent(QString);
    void tLog(QString);

private:
    int sheetSizeX, sheetSizeY;
    int sheetGridSize;
    QMenu* popupMenu;
    Mode sceneMode;
    QPointF origPoint;
    int numEndpoints;
    QGraphicsLineItem* outline;
    QGraphicsLineItem* lineToDraw;
    QGraphicsRectItem* rectToDraw;
    void makeItemsControllable(bool areControllable);

};

#endif // SCHEMATICSHEET_H
