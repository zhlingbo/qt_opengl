#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lightSettingWidget(new LightSettingWidget)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->openGLWidget);

    connect(lightSettingWidget, &LightSettingWidget::opened_signal, ui->openGLWidget, &Render::dirLightOpenedSlot);
    connect(lightSettingWidget, &LightSettingWidget::strength_specular_value_signal, ui->openGLWidget, &Render::dirLightStrengthSpecular);
    connect(lightSettingWidget, &LightSettingWidget::strength_diffuse_value_signal, ui->openGLWidget, &Render::dirLightStrengthDiffuse);
    connect(lightSettingWidget, &LightSettingWidget::strength_ambient_value_signal, ui->openGLWidget, &Render::dirLightStrengthAmbient);
    connect(lightSettingWidget, &LightSettingWidget::direction_x_changed, ui->openGLWidget, &Render::dirLight_x);
    connect(lightSettingWidget, &LightSettingWidget::direction_y_changed, ui->openGLWidget, &Render::dirLight_y);
    connect(lightSettingWidget, &LightSettingWidget::direction_z_changed, ui->openGLWidget, &Render::dirLight_z);

    connect(lightSettingWidget, &LightSettingWidget::point_light_opened, ui->openGLWidget, &Render::pointLightOpenedSlot);
    connect(lightSettingWidget, &LightSettingWidget::strength_specular_value_point, ui->openGLWidget, &Render::pointLightStrengthSpecular);
    connect(lightSettingWidget, &LightSettingWidget::strength_diffuse_value_point, ui->openGLWidget, &Render::pointLightStrengthDiffuse);
    connect(lightSettingWidget, &LightSettingWidget::strength_ambient_value_point, ui->openGLWidget, &Render::pointLightStrengthAmbient);

    ui->dockWidget->hide();
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


void MainWindow::on_actLightSetting_triggered()
{
    ui->dockWidget->setWidget(lightSettingWidget);
    ui->dockWidget->show();
}

