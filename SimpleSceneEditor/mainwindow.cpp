#include <QMenu>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schematicscene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sheet = new SchematicScene(this);
    sheet->setSceneRect(0,0,2000,2000);
    ui->graphicsView->setScene(sheet);

    ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(ui->graphicsView, SIGNAL(customContextMenuRequested(QPoint)),
//            this, SLOT(customMenuRequested(QPoint)));
    connect(sheet, SIGNAL(statusEvent(QString)), this, SLOT(setStatusBarText(QString)));
    connect(ui->graphicsView, SIGNAL(tLog(QString)), this, SLOT(tLog(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::customMenuRequested(QPoint pos){
    sheet->customMenuRequested(pos);
}

void MainWindow::tLog(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}

void MainWindow::setStatusBarText(QString text)
{
    ui->statusBar->showMessage(text);
}

void MainWindow::on_actionQuit_triggered()
{

}

