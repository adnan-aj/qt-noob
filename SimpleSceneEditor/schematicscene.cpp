#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QtGui>
#include <QMenu>
#include <QKeySequence>

#include "schematicscene.h"
//#include "customrectitem.h"
//#include "customlineitem.h"

SchematicScene::SchematicScene(QObject* parent): QGraphicsScene(parent), sheetSizeX(16500), sheetSizeY(11700), sheetGridSize(100)
{
    sceneMode = NoMode; //enum Mode {NoMode, SelectObject, AddDot, DrawLine, DrawLine1, DrawLine2};
    lineToDraw = 0;
    rectToDraw = 0;
    popupMenu = new QMenu("Custom");
    //QKeySequence* adddot = new QKeySequence(Qt::Key_D);
    QAction* pAction1 = new QAction("Add Dot", this);
    pAction1->setShortcut(Qt::Key_D);
    QAction* pAction2 = new QAction("Add Line", this);
    QAction* pAction3 = new QAction("Add Rect", this);
    popupMenu->addAction(pAction1);
    popupMenu->addAction(pAction2);
    popupMenu->addAction(pAction3);
    connect(pAction1, SIGNAL(triggered()), this, SLOT(action1()));
    connect(pAction2, SIGNAL(triggered()), this, SLOT(action2()));
    connect(pAction3, SIGNAL(triggered()), this, SLOT(action3()));

    /* sheet size */
    this->setSceneRect(-2000, -2000, sheetSizeX+4000, sheetSizeY+4000);
    QBrush sheetColor(QColor(255,252,248));
    QPen sheetBorderColor(Qt::black);
    sheetBorderColor.setWidth(10);

    /* "tabletop" background coloring */
    QColor backgroundTopColor(Qt::lightGray);
    QColor backgroundBottomColor(Qt::darkGray);
    /* top-to-bottom of sheet outline */
    QLinearGradient shade(0, 0, 0, this->sceneRect().height());
    shade.setColorAt(0,backgroundTopColor);
    shade.setColorAt(1, backgroundBottomColor);
    this->setBackgroundBrush(shade);

    /* primitive way of drawing sheet */
    QGraphicsItem *sheetItem;
//    sheetItem = this->addRect(200, 200, sheetSizeX-400, sheetSizeY-400, sheetBorderColor);
//    sheetItem->setFlag(QGraphicsItem::ItemIsMovable, false);
//    sheetItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    QGraphicsItem *borderItem;
//    borderItem = this->addRect(400, 400, sheetSizeX-800, sheetSizeY-800, sheetBorderColor);
//    borderItem->setFlag(QGraphicsItem::ItemIsMovable, false);
//    borderItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void SchematicScene::customMenuRequested(QPoint pos)
{
    QGraphicsView* mView = views().at(0);
    if (mView) {
        popupMenu->popup(mView->viewport()->mapToGlobal(pos));
    }
}

void SchematicScene::action1()
{
    setSceneMode(AddDot);
}

void SchematicScene::action2()
{
    setSceneMode(AddLine);
    numEndpoints = 0;
}

void SchematicScene::action3()
{
    setSceneMode(AddRect);
    rectToDraw = new QGraphicsRectItem(0, 0, 100, 100);
    this->addItem(rectToDraw);
    QGraphicsView* mView = views().at(0);
    if (mView) {
        mView->setMouseTracking(true);
    }
    numEndpoints = 0;
}

void SchematicScene::makeItemsControllable(bool areControllable)
{

}

void SchematicScene::setSceneMode(SchematicScene::Mode mode)
{
//    QGraphicsView::NoDrag           0	Nothing happens; the mouse event is ignored.
//    QGraphicsView::ScrollHandDrag	1	The cursor changes into a pointing hand,
//                                    and dragging the mouse around will scroll the
//                                    scrolbars. This mode works both in interactive
//                                    and non-interactive mode.
//    QGraphicsView::RubberBandDrag	2	A rubber band will appear. Dragging the
//                                    mouse will set the rubber band geometry, and
//                                    all items covered by the rubber band are
//                                    selected. This mode is disabled for non-
//                                    interactive views.
    sceneMode = mode;
    QGraphicsView::DragMode vMode = QGraphicsView::RubberBandDrag;
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);
}

void SchematicScene::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key == Qt::Key_Delete) {
        foreach (QGraphicsItem* item, selectedItems()) {
            removeItem(item);
            delete item;
        }
    }
    else if (key == Qt::Key_Escape) {
        if (sceneMode == AddDot)
            setSceneMode(NoMode);
        else if ((sceneMode == AddLine) || (sceneMode == AddRect)) {
            if (numEndpoints == 0) {
                setSceneMode(NoMode);
                if (lineToDraw)
                    this->removeItem(lineToDraw);
                if (rectToDraw)
                    this->removeItem(rectToDraw);
            }
            else if (numEndpoints == 1) {
                numEndpoints == 0;
                if (lineToDraw)
                    this->removeItem(lineToDraw);
            }
        }
    }
    else
        QGraphicsScene::keyPressEvent(event);
}

void SchematicScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QBrush background(Qt::darkGray);
    QColor gridColor(Qt::darkBlue);
    QPen gridPen(gridColor);
    gridPen.setWidth(5);
    //painter->setBackgroundMode(Qt::OpaqueMode);
    //painter->setBackground(background);
    //painter->setBrush(background);

    qreal left = int(rect.left()) - (int(rect.left()) % sheetGridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % sheetGridSize);
    emit tLog(tr("ViewZoom::drawBackground ") + QString::number(int(rect.width() / sheetGridSize)) + "," + QString::number(int(rect.height() / sheetGridSize)));

    // Paint the sheet background color first, with no sheet outline
    QGraphicsScene::drawBackground(painter, rect);\
    QBrush sheetColor(QColor(255,252,248));
    painter->setBrush(sheetColor);
    painter->setPen(Qt::NoPen);
    painter->drawRect(0, 0, sheetSizeX, sheetSizeY);

    // Paint the grid on all background TODO: paint grid only on the sheet
    painter->setPen(gridPen);
    QVector<QPointF> points;
    for (qreal x = left; x < rect.right(); x += sheetGridSize){
        for (qreal y = top; y < rect.bottom(); y += sheetGridSize){
            points.append(QPointF(x,y));
        }
    }
    painter->drawPoints(points.data(), points.size());
}

void SchematicScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF currPoint;
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    QPen blackPenDot(Qt::black);
    blackPenDot.setWidth(0);
    blackPenDot.setStyle(Qt::DashLine);
    QPen blackPen3(Qt::black);
    blackPen3.setWidth(3);
    QGraphicsItem* item;

    currPoint = event->scenePos();

    if (event->button() == Qt::LeftButton) {
        if (sceneMode == AddDot) {
            //addEllipse(qreal x, qreal y, qreal w, qreal h, const QPen &pen = QPen(), const QBrush &brush = QBrush())
            item = this->addEllipse(currPoint.x()-5, currPoint.y()-5, 10, 10, blackPen, redBrush);
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            event->accept();
        }
        else if (sceneMode == AddLine) {
            if (numEndpoints == 0) {
                origPoint = currPoint;
                lineToDraw = new QGraphicsLineItem;
                this->addItem(lineToDraw);
                lineToDraw->setPen(blackPenDot);
                lineToDraw->setPos(origPoint);
                numEndpoints = 1;
                QGraphicsView* mView = views().at(0);
                if (mView) {
                    mView->setMouseTracking(true);
                }

            }
            else if (numEndpoints == 1) {
                lineToDraw->setPen(blackPen3);
                lineToDraw->setFlag(QGraphicsItem::ItemIsMovable, true);
                lineToDraw->setFlag(QGraphicsItem::ItemIsSelectable, true);
                lineToDraw = 0;
                numEndpoints = 0;
                QGraphicsView* mView = views().at(0);
                if (mView) {
                    mView->setMouseTracking(false);
                }
            }
        }
        else if (sceneMode == AddRect) {
            if (numEndpoints == 0) {
                origPoint = currPoint;
                //rectToDraw = new QGraphicsRectItem(0, 0, 100, 100);
                //this->addItem(rectToDraw);
                //rectToDraw->setPen(blackPenDot);
                rectToDraw->setPos(origPoint);
                numEndpoints = 1;
                QGraphicsView* mView = views().at(0);
                if (mView) {
                    mView->setMouseTracking(true);
                }

            }
            else if (numEndpoints == 1) {
                rectToDraw->setPen(blackPen3);
                rectToDraw->setFlag(QGraphicsItem::ItemIsMovable, true);
                rectToDraw->setFlag(QGraphicsItem::ItemIsSelectable, true);
                rectToDraw = 0;
                numEndpoints = 0;
                QGraphicsView* mView = views().at(0);
                if (mView) {
                    mView->setMouseTracking(false);
                }
                action3();
            }
        }

    }
    QGraphicsScene::mousePressEvent(event);

}

void SchematicScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    emit statusEvent(tr("XY:") + QString::number(event->scenePos().x()) + tr(",") + QString::number(-event->scenePos().y()));
    if ((sceneMode == AddLine) && (numEndpoints == 1)) {
        lineToDraw->setLine(0,0,event->scenePos().x() - origPoint.x(),
                                event->scenePos().y() - origPoint.y());
    }
    else if (sceneMode == AddRect) {
        if (numEndpoints == 0) {
             rectToDraw->setPos(event->scenePos().x(), event->scenePos().y());
        }
        else if (numEndpoints == 1) {
            rectToDraw->setRect(0,0,event->scenePos().x() - origPoint.x(),
                                    event->scenePos().y() - origPoint.y());
        }
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}
