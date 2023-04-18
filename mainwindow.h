#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "dockwidgets/lightsettingwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actImportModel_triggered();

    void on_actWireFrame_triggered();

    void on_actFillColor_triggered();

    void on_actPointLight_triggered();

    void on_actDirectLight_triggered();

    void on_actLightOff_triggered();

    void on_actLightSetting_triggered();

private:
    Ui::MainWindow *ui;
    LightSettingWidget *lightSettingWidget;
};
#endif // MAINWINDOW_H
