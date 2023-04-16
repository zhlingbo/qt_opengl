#include "model.h"

Model::Model(QOpenGLFunctions_3_3_Core *glfuns,const char *path)
    : m_glFuns(glfuns)
{
    loadModel(path);
    setBBox();
}

void Model::Draw(QOpenGLShaderProgram &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP::" << import.GetErrorString();
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::setBBox()
{
    bbox = meshes[0].bbox;
    for (unsigned int i = 1; i < meshes.size(); i++) {
        QVector3D& meshMax = meshes[i].bbox.max;
        if (bbox.max.x() < meshMax.x()) bbox.max.setX(meshMax.x());
        if (bbox.max.y() < meshMax.y()) bbox.max.setY(meshMax.y());
        if (bbox.max.z() < meshMax.z()) bbox.max.setZ(meshMax.z());

        QVector3D& meshMin = meshes[i].bbox.min;
        if (bbox.min.x() > meshMin.x()) bbox.min.setX(meshMin.x());
        if (bbox.min.y() > meshMin.y()) bbox.min.setY(meshMin.y());
        if (bbox.min.z() > meshMin.z()) bbox.min.setZ(meshMin.z());
    }
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        if (bbox.max.x() < mesh->mVertices[i].x) bbox.max.setX(mesh->mVertices[i].x);
        if (bbox.max.y() < mesh->mVertices[i].y) bbox.max.setY(mesh->mVertices[i].y);
        if (bbox.max.z() < mesh->mVertices[i].z) bbox.max.setZ(mesh->mVertices[i].z);

        if (bbox.min.x() > mesh->mVertices[i].x) bbox.min.setX(mesh->mVertices[i].x);
        if (bbox.min.y() > mesh->mVertices[i].y) bbox.min.setY(mesh->mVertices[i].y);
        if (bbox.min.z() > mesh->mVertices[i].z) bbox.min.setZ(mesh->mVertices[i].z);

        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.Position = vector;

        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.Normal = vector;

        if (mesh->mTextureCoords[0]) {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = QVector2D(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // 处理材质
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps =
                loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps =
                loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(m_glFuns, vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    QOpenGLTexture* texture = new QOpenGLTexture(QImage(filename.c_str()).mirrored());
    if (texture == NULL) {
        qDebug() << "texture is NULL";
    } else {
        qDebug() << filename.c_str() << "loaded";
    }

    return texture->textureId();
}
