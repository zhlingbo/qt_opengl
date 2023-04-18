#ifndef LIGHTSETTINGWIDGET_H
#define LIGHTSETTINGWIDGET_H

#include <QWidget>

namespace Ui {
class LightSettingWidget;
}

class LightSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LightSettingWidget(QWidget *parent = nullptr);
    ~LightSettingWidget();

signals:
    void opened_signal(bool);
    void strength_specular_value_signal(float);
    void strength_diffuse_value_signal(float);
    void strength_ambient_value_signal(float);
    void direction_x_changed(float);
    void direction_y_changed(float);
    void direction_z_changed(float);

private slots:
    void on_opened_stateChanged(int opened);

    void on_strength_specular_valueChanged(int value);

    void on_strength_diffuse_valueChanged(int value);

    void on_strength_ambient_valueChanged(int value);

    void on_dx_valueChanged(double arg1);

    void on_dy_valueChanged(double arg1);

    void on_dz_valueChanged(double arg1);

private:
    Ui::LightSettingWidget *ui;
};

#endif // LIGHTSETTINGWIDGET_H
