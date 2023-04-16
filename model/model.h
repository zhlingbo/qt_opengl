#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLTexture>

#include "mesh.h"
#include "bbox.h"

class Model {
public:
    Model(QOpenGLFunctions_3_3_Core *glfuns,const char *path);
    void Draw(QOpenGLShaderProgram &shader);

    BBox bbox;
    vector<Texture> textures_loaded;

private:
    // model data
    QOpenGLFunctions_3_3_Core *m_glFuns;
    vector<Mesh> meshes;
    string directory;
    void loadModel(string path);
    void setBBox();

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    unsigned int TextureFromFile(const char *path, const string &directory);
};

#endif // MODEL_H
