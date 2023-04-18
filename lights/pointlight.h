#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <QVector3D>
#include <QVector4D>

struct PointLight {
    QVector4D position;
    bool opened;

    QVector3D color;

    float strength_specular;
    float strength_diffuse;
    float strength_ambient;

    float constant;
    float linear;
    float quadratic;

    PointLight()
        : position(QVector4D())
        , opened(false)
        , color(QVector3D(1.0, 1.0, 1.0))
        , strength_specular(1.0)
        , strength_diffuse(0.9)
        , strength_ambient(0.4)
        , constant(1.0)
        , linear(0.09)
        , quadratic(0.032)
    {}
};


#endif // POINTLIGHT_H
