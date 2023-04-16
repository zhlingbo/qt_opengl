#ifndef MESH_H
#define MESH_H

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <string>
#include <vector>

#include "bbox.h"
using namespace std;
struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};
struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    BBox bbox;

    void Draw(QOpenGLShaderProgram &shader);
    Mesh(QOpenGLFunctions_3_3_Core *glFuns, vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
private:
    // render data
    unsigned int m_VAO, m_VBO, m_EBO;
    void setupMesh();
    void setBBox();
private:
    QOpenGLFunctions_3_3_Core *m_glFuns;
};


#endif // MESH_H
