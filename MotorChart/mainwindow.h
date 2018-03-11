#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCore/QDateTime>
QT_CHARTS_USE_NAMESPACE

#include "odometer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Journey journey;
    QChart *movementChart;
    QDateTimeAxis *axisX;
    QLineSeries *Lseries, *Rseries;
    bool chartScroll_isScrolling;
    int chartScroll_prevValue;

public slots:
    void tLog(QString);

private slots:
    void on_actionOpen_triggered();
    void on_rangeChanged( qreal min, qreal max );
    void on_chartScroll_valueChanged(int value);
};

#endif // MAINWINDOW_H
