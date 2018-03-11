#include "odometer.h"

Odometer::Odometer()
{

}

Journey::Journey()
{
    LwheelPPR = RwheelPPR = 720;
    axleWidth = 425.0;
    LwheelCircumference = RwheelCircumference = 405.0;
}


int Journey::read(QString fileName)
{
    int lines = 0;
    QDateTime currentTime;
    Odometer location;
    int left, right;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        //QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        return -1;
    }

    this->clear();
    QTextStream in(&file);
    while (!in.atEnd()) {

        QString odoLine = in.readLine();
        lines++;

        QStringList parsed = odoLine.split(" ", QString::SplitBehavior::SkipEmptyParts);
        if (parsed.count() > 0) {
            if (parsed[0].toUpper() == "TIME:" && parsed.count() >= 3) {
                currentTime = QDateTime::fromString(parsed[1] + " " + parsed[2], "yyyy-MM-dd HH:mm:ss");
                location.time = currentTime;
                location.setting = QPoint(0,0);
                location.speed = QPoint(0,0);
                location.correction = QPoint(0,0);
                location.magnetic = 0.0;
                location.position = QPointF(0,0);
                location.heading = 0.0;
                //DO NOT this->append(location); this is a time event
            }
            else if (parsed[0].toUpper() == "ODO:" && parsed.count() >= 7) {
                currentTime = currentTime.addMSecs(100);
                location.time = currentTime;
                left = parsed[1].toInt();
                right = parsed[4].toInt();
                location.setting = QPoint(left, right);
                left = parsed[2].toInt();
                right = parsed[5].toInt();
                location.speed = QPoint(left, right);
                left = parsed[3].toInt();
                right = parsed[6].toInt();
                location.correction = QPoint(left, right);
                if (parsed.count() == 8)
                    location.magnetic = parsed[7].toFloat();
                else
                    location.magnetic = 0.0;
                location.position = QPointF(0,0);
                location.heading = 0.0;
                this->append(location);
            }
        }
    }
    file.close();
    calculatePoints();
    return lines;
}

static inline double wrapAngle( double angle )
{
    double twoPi = 2.0 * 3.141592865358979;
    return angle - twoPi * floor( angle / twoPi );
}

void Journey::addToScene(QGraphicsScene *scene)
{
    Odometer location;
    double scale = 1.0 / 50.0;
    QBrush brush(Qt::white);
    QPen pen(Qt::blue);
    QPen blackPen(Qt::black);
    pen.setWidth(1);

    for (int i = 0; i < this->count(); i++) {
        location = this->at(i);
        scene->addRect(location.position.x() * scale, location.position.y() * -scale, 2, 2, blackPen, brush);
        scene->addRect(i, i, 2, 2, pen, brush);
    }
}

void Journey::calculatePoints()
{

    LwheelPPR = 720;
    RwheelPPR = 720;

    axleWidth = 425.0;
    LwheelCircumference = 405.0;
    RwheelCircumference = 405.0;

    if (this->count() < 2)
        return;

    Odometer *selfData = this->data();

    for (int i = 1; i < this->count(); i++) {
        Odometer previous = this->at(i - 1);
        QPointF position = previous.position;
        double heading = previous.heading;
        int leftPulses = previous.speed.x();
        int rightPulses = previous.speed.y();

        double leftDelta = leftPulses * LwheelCircumference / LwheelPPR;
        double rightDelta = rightPulses * RwheelCircumference / RwheelPPR;

        float new_x, new_y, new_heading;
        if (fabs(leftDelta - rightDelta) < 1.0e-6) {
            //basically going straight
            new_x = position.x() + leftDelta * cos(heading);
            new_y = position.y() + rightDelta * sin(heading);
            new_heading = heading;
        }
        else {
            double R = axleWidth * (leftDelta + rightDelta) / (2 * (rightDelta - leftDelta));
            double wd = (rightDelta - leftDelta) / axleWidth;
            new_x = position.x() + R * sin(wd + heading) \
                                                - R * sin(heading);
            new_y = position.y() - R * cos(wd + heading) \
                                                + R * cos(heading);
            new_heading = (float) wrapAngle(heading + wd);
        }

        selfData[i].position.setX(new_x);
        selfData[i].position.setY(new_y);
        selfData[i].heading = new_heading;
    }
}
