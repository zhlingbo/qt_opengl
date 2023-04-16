#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->openGLWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actImportModel_triggered()
{
    QString str=QFileDialog::getOpenFileName(this, "选择模型文件", "", "OBJ (*.obj);;FBX(*.fbx);;ALL FILES( *.* ) ");
    ui->openGLWidget->loadModel(str.toStdString());
}


void MainWindow::on_actWireFrame_triggered()
{
    ui->openGLWidget->setWireFrame(true);
}


void MainWindow::on_actFillColor_triggered()
{
    ui->openGLWidget->setWireFrame(false);
}


void MainWindow::on_actPointLight_triggered()
{
    ui->openGLWidget->setLight(Render::PointLight);
}


void MainWindow::on_actDirectLight_triggered()
{
    ui->openGLWidget->setLight(Render::DirectLight);
}


void MainWindow::on_actLightOff_triggered()
{
    ui->openGLWidget->clearLight();
}

