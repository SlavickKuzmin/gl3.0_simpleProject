//
// Created by slavickkuzmin on 8/1/16.
//

#ifndef GL3_0_SHADERSTOOL_H
#define GL3_0_SHADERSTOOL_H

#include <GL/gl.h>

class Shaders {
public:
    Shaders(const char *vertex_file_path, const char *fragment_file_path);
    ~Shaders();
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint ProgramID;
};
#endif //GL3_0_SHADERSTOOL_H
