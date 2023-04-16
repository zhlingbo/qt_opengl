#include "camera.h"


Camera::Camera(QVector3D position, QVector3D up, float yaw, float pitch) :
    position(position),
    worldUp(up),
    front(-position),
    pitch(pitch),
    yaw(yaw),
    zoom(Zoom),
    viewOrigin(0, 0, 0)
{
    this->updateCameraVectors();
}

Camera::~Camera()
{

}

QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 view;
    view.lookAt(this->position, this->viewOrigin, this->up);
    return view;
}

void Camera::changeZoom(float dt)
{
    if (this->zoom >= 1.0f && this->zoom <= 89.0f)
        this->zoom += dt;
    if (this->zoom > 89.0f)
        this->zoom = 89.0f;
    if (this->zoom < 1.0f)
        this->zoom = 1.0f;
}

void Camera::moveForward(float dt)
{
    this->position = this->position + this->front * dt;
    this->updateCameraVectors();
}

void Camera::moveAround(float dx, float dy)
{
    QVector3D roundR = this->position - this->viewOrigin;
    this->position = this->viewOrigin + (roundR + this->up * dy + this->right * dx).normalized() * roundR.length();
    this->updateCameraVectors();
}

void Camera::moveFlat(float dx, float dy)
{
    QVector3D dp = this->up * dy + this->right * dx;
    this->position = this->position + dp;
    this->viewOrigin = this->viewOrigin + dp;
}

float Camera::getZoom() const
{
    return this->zoom;
}

QVector3D Camera::getPosition() const
{
    return this->position;
}

void Camera::updateCameraVectors()
{
    QVector3D front;
    front = this->viewOrigin - this->position;
    this->front = front.normalized();
    this->right = QVector3D::crossProduct(this->front, this->worldUp).normalized();
    this->up = QVector3D::crossProduct(this->right, this->front).normalized();
}
