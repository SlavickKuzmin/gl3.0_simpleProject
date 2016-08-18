#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>
#include <vector>


#include <map>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "util.h"
#include "math_3d.h"
#include "texture.h"

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};


class Mesh
{
public:
    Mesh(GLuint pid);

    ~Mesh();

    bool LoadMesh(const std::string& Filename);

    void Render();

private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();
#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

        void Init(const std::vector<Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;

        GLuint VertexArrayID;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;

    GLuint programID;
};

/*
class LoadOBJModel {
public:
    LoadOBJModel(const char *filepath, GLuint &pid);
    ~LoadOBJModel();
    void InitDraw();
    void draw();
    GLuint loadTextureBMP(const char * imagepath);
    GLuint loadTextureDDS(const char * imagepath);
    GLuint getTexture();
private:
    GLuint programID;
    //
    std::vector<glm::vec3> *vertices;
    std::vector<glm::vec2> *uvs;
    std::vector<glm::vec3> *normals;
    //
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    //text
    GLuint Texture;
    GLuint TextureID;
};
*/

#endif