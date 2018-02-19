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
    sheet->setSceneRect(0,0,200,200);
    ui->graphicsView->setScene(sheet);
    sheet->setSceneMode(SchematicScene::NoMode);

    ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->graphicsView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customMenuRequested(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::customMenuRequested(QPoint pos){
    sheet->customMenuRequested(pos);
}

void MainWindow::on_actionQuit_triggered()
{

}
