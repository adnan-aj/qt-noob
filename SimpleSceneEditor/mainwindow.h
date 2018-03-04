#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "schematicscene.h"

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
    SchematicScene *sheet;


public slots:
    void customMenuRequested(QPoint pos);
    void tLog(QString);
    void setStatusBarText(QString);

private slots:
    void on_actionQuit_triggered();
};

#endif // MAINWINDOW_H
