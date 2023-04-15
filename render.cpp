#include "render.h"

Render::Render(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_camera(QVector3D(5.0f, 0.0f, 10.0f))
{
    setFocusPolicy(Qt::StrongFocus);
}

Render::~Render()
{
    if (m_model != nullptr) {
        delete m_model;
        m_model = nullptr;
    }
}

void Render::loadModel(string path)
{
    if (m_model != nullptr)
        delete m_model;

    m_model = nullptr;
    makeCurrent();
    m_model = new Model(QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext()), path.c_str());
    m_camera = Camera(cameraPosInit(m_model->m_maxY, m_model->m_minY));
    doneCurrent();
}

void Render::setWireFrame(bool isWireFrame)
{
    makeCurrent();
    if (isWireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    doneCurrent();
    update();
}

void Render::initializeGL()
{
    initializeOpenGLFunctions();

    bool success;
    m_ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shapes.vert");
    m_ShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shapes.frag");
    success = m_ShaderProgram.link();
    if(!success)
        qDebug() << "ERR:" << m_ShaderProgram.log();

    glEnable(GL_DEPTH_TEST);
}

void Render::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Render::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_model == nullptr) return;

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    projection.perspective(m_camera.getZoom(), (float)width()/height(), 0.1f, 100.0f);
    view = m_camera.getViewMatrix();

    m_ShaderProgram.bind();
    m_ShaderProgram.setUniformValue("projection", projection);
    m_ShaderProgram.setUniformValue("view", view);

    m_ShaderProgram.setUniformValue("viewPos", m_camera.getPosition());

    // light properties, note that all light colors are set at full intensity
    m_ShaderProgram.setUniformValue("light.ambient", 0.4f, 0.4f, 0.4f);
    m_ShaderProgram.setUniformValue("light.diffuse", 0.9f, 0.9f, 0.9f);
    m_ShaderProgram.setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    m_ShaderProgram.setUniformValue("material.shininess", 32.0f);
    m_ShaderProgram.setUniformValue("light.direction", -0.2f, -1.0f, -0.3f);

    m_ShaderProgram.setUniformValue("model", model);
    m_model->Draw(m_ShaderProgram);
}

void Render::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();
    m_camera.changeZoom(-offset.y()/20.0f);
    update();
}

void Render::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        m_bMouseRightPressed = true;
        m_lastPos = event->position().toPoint();
    }
    if (event->button() == Qt::MiddleButton) {
        m_bMouseMiddlePressed = true;
        m_lastPos = event->position().toPoint();
    }
}

void Render::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bMouseMiddlePressed = false;
    m_bMouseRightPressed = false;
}

void Render::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseRightPressed) {
        int xPos = event->position().toPoint().x();
        int yPos = event->position().toPoint().y();

        float xoffset = xPos - m_lastPos.x();
        float yoffset = m_lastPos.y() - yPos;
        m_lastPos = event->position().toPoint();
        m_camera.moveAround(-xoffset, -yoffset);
    }
    if (m_bMouseMiddlePressed) {
        int xPos = event->position().toPoint().x();
        int yPos = event->position().toPoint().y();

        float xoffset = (xPos - m_lastPos.x())*0.1f;
        float yoffset = (m_lastPos.y() - yPos)*0.1f;
        m_lastPos = event->position().toPoint();
        m_camera.moveFlat(-xoffset, -yoffset);
    }
    update();
}

void Render::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
        m_camera = Camera(cameraPosInit(m_model->m_maxY, m_model->m_minY));
    update();
}

QVector3D Render::cameraPosInit(float maxY, float minY)
{
    QVector3D temp = {0,0,0};
    float height = maxY - minY;
    temp.setZ(1.5 * height);
    if (minY >= 0)
        temp.setY(height / 2.0);
    return temp;
}
