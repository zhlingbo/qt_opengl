#ifndef RENDER_H
#define RENDER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class Render : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
public:
    explicit Render(QWidget* parent = 0);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};

#endif // RENDER_H
