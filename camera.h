#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

const float Yaw = -90.0f;
const float Pitch = 0.0f;
const float Speed = 1.0f;
const float Sensitivity = 0.01f;
const float Zoom = 45.0f;

class Camera
{
public:
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 0.1f, 0.0f),
           float yaw = Yaw, float pitch = Pitch);
    ~Camera();

    QMatrix4x4 getViewMatrix();
    void changeZoom(float dt);
    void moveAround(float dx, float dy);
    void moveFlat(float dx, float dy);
    float getZoom() const;
    QVector3D getPosition() const;

private:
    void updateCameraVectors();

    QVector3D position;
    QVector3D worldUp;
    QVector3D front;

    QVector3D up;
    QVector3D right;

    QVector3D viewOrigin;

    float pitch;
    float yaw;
    float zoom;
};

#endif // CAMERA_H
