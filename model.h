#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <glm/glm.hpp>
#include <vector>

class LoadOBJModel {
public:
    LoadOBJModel(const char *filepath);
    ~LoadOBJModel();
    std::vector<glm::vec3> *vertices;
    std::vector<glm::vec2> *uvs;
    std::vector<glm::vec3> *normals;
};

#endif