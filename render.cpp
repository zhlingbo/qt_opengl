#include "render.h"

Render::Render(QWidget* parent) : QOpenGLWidget(parent)
{

}

void Render::initializeGL()
{
    initializeOpenGLFunctions();
}

void Render::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Render::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
