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
    if (m_model != nullptr) {
        delete m_model;
        m_model = nullptr;
    }
    makeCurrent();

    m_model = new Model(QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext()), path.c_str());

    m_camera = Camera(cameraPosInit(m_model->bbox));
    pointLight.position = QVector4D(m_model->bbox.max, 1.0f);

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
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

    m_ShaderProgram.setUniformValue("viewPos", QVector4D(m_camera.getPosition(), 1.0f));

    if (directionLight.opened) {
        m_ShaderProgram.setUniformValue("dirLight.direction", directionLight.direction);
        m_ShaderProgram.setUniformValue("dirLight.ambient", directionLight.strength_ambient * directionLight.color);
        m_ShaderProgram.setUniformValue("dirLight.diffuse", directionLight.strength_diffuse * directionLight.color);
        m_ShaderProgram.setUniformValue("dirLight.specular", directionLight.strength_specular * directionLight.color);
    } else {
        m_ShaderProgram.setUniformValue("dirLight.direction", QVector3D());
    }

    if (pointLight.opened) {
        m_ShaderProgram.setUniformValue("pointLight.position", pointLight.position);
        m_ShaderProgram.setUniformValue("pointLight.ambient", pointLight.strength_ambient * pointLight.color);
        m_ShaderProgram.setUniformValue("pointLight.diffuse", pointLight.strength_diffuse * pointLight.color);
        m_ShaderProgram.setUniformValue("pointLight.specular", pointLight.strength_specular * pointLight.color);

        m_ShaderProgram.setUniformValue("pointLight.constant", pointLight.constant);
        m_ShaderProgram.setUniformValue("pointLight.linear", pointLight.linear);
        m_ShaderProgram.setUniformValue("pointLight.quadratic", pointLight.quadratic);
    } else {
        m_ShaderProgram.setUniformValue("pointLight.position", QVector4D());
    }

    // material properties
    m_ShaderProgram.setUniformValue("material.shininess", 32.0f);

    m_ShaderProgram.setUniformValue("model", model);
    m_model->Draw(m_ShaderProgram);
}

void Render::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();
    m_camera.changeZoom(-offset.y() / 20.0f);
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

        float xoffset = (xPos - m_lastPos.x()) * 0.1f;
        float yoffset = (m_lastPos.y() - yPos) * 0.1f;
        m_lastPos = event->position().toPoint();
        m_camera.moveFlat(-xoffset, -yoffset);
    }
    update();
}

void Render::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
        m_camera = Camera(cameraPosInit(m_model->bbox));
    update();
}

QVector3D Render::cameraPosInit(BBox bbox)
{
    float x = (bbox.max.x() - bbox.min.x()) / 2;
    float y = (bbox.max.y() - bbox.min.y()) / 2;
    float z = (bbox.max.y() - bbox.min.y()) * 2;
    return QVector3D(x, y, z);
}

void Render::dirLightOpenedSlot(bool opened)
{
    directionLight.opened = opened;
    update();
}

void Render::dirLightStrengthSpecular(float strength)
{
    directionLight.strength_specular = strength;
    update();
}

void Render::dirLightStrengthDiffuse(float strength)
{
    directionLight.strength_diffuse = strength;
    update();
}

void Render::dirLightStrengthAmbient(float strength)
{
    directionLight.strength_ambient = strength;
    update();
}

void Render::dirLight_x(float x)
{
    directionLight.direction.setX(x);
    update();
}

void Render::dirLight_y(float y)
{
    directionLight.direction.setY(y);
    update();
}

void Render::dirLight_z(float z)
{
    directionLight.direction.setZ(z);
    update();
}

void Render::pointLightOpenedSlot(bool opened)
{
    pointLight.opened = opened;
    update();
}

void Render::pointLightStrengthSpecular(float strength)
{
    pointLight.strength_specular = strength;
    update();
}

void Render::pointLightStrengthDiffuse(float strength)
{
    pointLight.strength_diffuse = strength;
    update();
}

void Render::pointLightStrengthAmbient(float strength)
{
    pointLight.strength_ambient = strength;
    update();
}
