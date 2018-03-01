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

private slots:
    void on_actionQuit_triggered();
    void setStatusBarText(const QString&);

};

#endif // MAINWINDOW_H
