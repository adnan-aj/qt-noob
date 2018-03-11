#ifndef ODOMETER_H
#define ODOMETER_H

#include <QtCore>
#include <QtGui>
#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>


/* visual example :P */
class parkingCalc{
protected:
float rate;
float firstHour;
public:
//void set_rate(float set);
//void set_firstHour(float set);
//void get_total(int hours);
};

class Odometer
{
public:
    Odometer();
    QDateTime time;
    QPointF position;
    QPoint setting;
    QPoint speed;
    QPoint correction;
    float magnetic;
    double heading;
};

class Journey : public QVector<Odometer>
{
public:
    Journey();
    //QVector<Odometer> _points;
    int read(QString);
    void calculatePoints();
    void addToScene(QGraphicsScene *);

#if 0
    int LwheelPPR();
    int RwheelPPR();
    float axleWidth();
    float LwheelCircumference();
    float RwheelCircumference();
#endif

private:
    int LwheelPPR, RwheelPPR;
    double axleWidth, LwheelCircumference, RwheelCircumference;
};

#endif // ODOMETER_H
