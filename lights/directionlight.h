#ifndef DIRECTIONLIGHT_H
#define DIRECTIONLIGHT_H

#include <QVector3D>

struct DirectionLight {
    QVector3D direction;
    bool opened;
    QVector3D color;
    float strength_specular;
    float strength_diffuse;
    float strength_ambient;

    DirectionLight()
        : direction(QVector3D(-0.2, -1.0, -0.3))
        , opened(true)
        , color(QVector3D(1.0, 1.0, 1.0))
        , strength_specular(1.0)
        , strength_diffuse(0.9)
        , strength_ambient(0.4)
    {}
};

#endif // DIRECTIONLIGHT_H
