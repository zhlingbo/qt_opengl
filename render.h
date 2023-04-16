#ifndef RENDER_H
#define RENDER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QWheelEvent>
#include <QMouseEvent>

#include "camera.h"
#include "model/model.h"

class Render : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
public:
    explicit Render(QWidget* parent = 0);
    ~Render();

    void loadModel(string path);
    void setWireFrame(bool isWireFrame);
    void setLight(unsigned int lightMode);
    void clearLight();

    enum LightMode {
        DirectLight,
        PointLight
    };

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
};

#endif // RENDER_H
