#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Journey journey();
    movementChart = new QChart();
    movementChart->setTitle("Odometer");
    movementChart->legend()->hide();

    Lseries = new QLineSeries();
    Rseries = new QLineSeries();

    Lseries->append(0.0, 0.0);
    Lseries->append(15000.0, 0.0);
    Rseries->append(0.0, 20.0);
    Rseries->append(15000.0, 20.0);

    axisX = new QDateTimeAxis;
    axisX->setTitleText("Time");
    axisX->setFormat("HH:mm:ss");
    axisX->setTickCount(10);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Wheels");
//    axisY->setLabelFormat("%i");

    movementChart->addAxis(axisX, Qt::AlignBottom);
    movementChart->addAxis(axisY, Qt::AlignLeft);
    movementChart->addSeries(Lseries);
    movementChart->addSeries(Rseries);
    Lseries->attachAxis(axisX);
    Rseries->attachAxis(axisX);
    Lseries->attachAxis(axisY);
    Rseries->attachAxis(axisY);

    QDateTime timeNow = QDateTime::currentDateTime();
    movementChart->axisX()->setRange(timeNow, timeNow.addSecs(300));
    movementChart->axisY()->setRange(-200, 200);
    ui->chartView->setChart(movementChart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    //ui->chartView->setRubberBand( QChartView::HorizontalRubberBand );

    connect( movementChart->axisX( ), SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(on_rangeChanged(qreal,qreal)) );
    // DONE::::
    // https://stackoverflow.com/questions/40832713/enabling-horizontal-scaling-scrolling-on-qchartview-nov-2016-qt-5-7-0
    // TODO::::
    // https://stackoverflow.com/questions/46805186/qt-chart-move-view-with-pressed-middle-mouse-button
    // https://stackoverflow.com/questions/48623595/scale-x-axis-of-qchartview-using-mouse-wheel
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tLog(QString line)
{
    ui->plainTextEdit->appendPlainText(line);
}

void MainWindow::on_actionOpen_triggered()
{
    int lineCount;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), \
                                                tr("Text Files (*.txt);;C++ Files(*.cpp *.h))"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        lineCount = journey.read(fileName);
        tLog(tr("File loaded ") + QString::number(lineCount) + tr(" lines"));
    }
    //journey.calculatePoints();

    QVector<QPointF> Lpoints, Rpoints;
    for (int i = 0; i < journey.count(); i++) {
        qreal t = journey[i].time.toMSecsSinceEpoch();
        Lpoints.append(QPointF(t, (qreal)journey[i].speed.x()));
        Rpoints.append(QPointF(t, (qreal)journey[i].speed.y()));
    }

    Lseries->replace(Lpoints);
    Rseries->replace(Rpoints);
    movementChart->axisX()->setRange(journey.first().time, journey.last().time);
    movementChart->axisY()->setRange(-200, 200);
    movementChart->update();
}

void MainWindow::on_chartScroll_valueChanged(int value)
{
//    https://stackoverflow.com/questions/40832713/enabling-horizontal-scaling-scrolling-on-qchartview-nov-2016-qt-5-7-0
    if (!chartScroll_isScrolling) {
        chartScroll_isScrolling = true;
//      qDebug() << "scroll " << value << endl;
        ui->chartView->chart()->scroll( value - chartScroll_prevValue, 0 );
        chartScroll_prevValue = value;
        chartScroll_isScrolling = false;
    }
}

void MainWindow::on_rangeChanged( qreal min, qreal max )
{
    if (chartScroll_isScrolling)
        return;  // Scrolling causes range changes, but we don't have to do anything.

    QChart          *c = ui->chartView->chart( );
    QAbstractAxis   *x = c->axisX( );
    qreal            avg = (min + max) / 2.0;
    bool             range_fixed = false;

    /*
     * Make sure the new range is sane; fix if not.
     */
    if ((max - min) < 0.1) {    // Avoid overzooming
        min = avg - 0.05;
        max = avg + 0.05;
        range_fixed = true;
    }

    if (min < 0.0) { min = 0.0; range_fixed = true; }

    if (max > 4.0) { max = 4.0; range_fixed = true; }

    if (range_fixed) {
        x->setRange( min, max );    // will re-signal with the fixed range
        return;
    }

    qreal    vis_width = c->plotArea( ).width( );
    qreal    all_width = vis_width * (4.0 - 0.0) / (max - min);

//    cerr << "range " << min << " ... " << max << " in " << vis_width << " pixels" << endl;
//    cerr << "full width requires " << all_width << " pixels" << endl;;

    if (max - min < 4.0) {
//        cerr << "set scroll parameters" << endl;
        chartScroll_isScrolling = true;
        ui->chartScroll->setMaximum( all_width - vis_width );
        chartScroll_prevValue = min / (4.0 - 0.0) * all_width;
        ui->chartScroll->setValue( chartScroll_prevValue );
        chartScroll_isScrolling = false;
    } else {
//        cerr << "disable scroll bar" << endl;
        chartScroll_isScrolling = true;
        ui->chartScroll->setMaximum(0);
        ui->chartScroll->setValue(chartScroll_prevValue);
        chartScroll_isScrolling = false;
    }
}
