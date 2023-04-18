#include "lightsettingwidget.h"
#include "ui_lightsettingwidget.h"

LightSettingWidget::LightSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LightSettingWidget)
{
    ui->setupUi(this);
}

LightSettingWidget::~LightSettingWidget()
{
    delete ui;
}

void LightSettingWidget::on_opened_stateChanged(int opened)
{
    if (opened == Qt::Checked)
        emit opened_signal(true);
    else
        emit opened_signal(false);
}


void LightSettingWidget::on_strength_specular_valueChanged(int value)
{
    emit strength_specular_value_signal(value / 100.0f);
}


void LightSettingWidget::on_strength_diffuse_valueChanged(int value)
{
    emit strength_diffuse_value_signal(value / 100.0f);
}


void LightSettingWidget::on_strength_ambient_valueChanged(int value)
{
    emit strength_ambient_value_signal(value / 100.0f);
}


void LightSettingWidget::on_dx_valueChanged(double arg1)
{
    emit direction_x_changed(arg1);
}


void LightSettingWidget::on_dy_valueChanged(double arg1)
{
    emit direction_y_changed(arg1);
}


void LightSettingWidget::on_dz_valueChanged(double arg1)
{
    emit direction_z_changed(arg1);
}

