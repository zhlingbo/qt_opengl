#ifndef RENDER_H
#define RENDER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
//#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QTimer>

#include "camera.h"
#include "model/model.h"
#include "lights/directionlight.h"
#include "lights/pointlight.h"

class Render : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
public:
    explicit Render(QWidget* parent = 0);
    ~Render();

    void loadModel(string path);
    void setWireFrame(bool isWireFrame);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;


private:
    QVector3D cameraPosInit(BBox bbox);

    QOpenGLShaderProgram m_ShaderProgram;
    Camera m_camera;
    bool m_bMouseRightPressed;
    bool m_bMouseMiddlePressed;
    QPoint m_lastPos;

    Model* m_model = nullptr;

    DirectionLight directionLight;
    PointLight pointLight;

    // bool isPointLightRotate;
    QTimer* lightRotateTimer = nullptr;
    const unsigned int timeoutmSec = 50;

private slots:
    void light_rotate_timeout();

public slots:
    void dirLightOpenedSlot(bool opened);
    void dirLightStrengthSpecular(float strength);
    void dirLightStrengthDiffuse(float strength);
    void dirLightStrengthAmbient(float strength);
    void dirLight_x(float x);
    void dirLight_y(float y);
    void dirLight_z(float z);

    void pointLightOpenedSlot(bool opened);
    void pointLightStrengthSpecular(float strength);
    void pointLightStrengthDiffuse(float strength);
    void pointLightStrengthAmbient(float strength);
    void pointLightRotate(bool rotate);
};

#endif // RENDER_H
