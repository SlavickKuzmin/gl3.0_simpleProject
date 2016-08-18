//
// Created by slavickkuzmin on 8/15/16.
//

#ifndef GL3_0_WINDOW_H
#define GL3_0_WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//fix it?
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Window {
public:
    Window(const char *name, bool mode);
    ~Window();
    GLFWwindow* getWindow();
    //==========TOOL========
    void setBackground(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { glClearColor(r, g, b, a); }

    void zBufferOnOff(bool on);
    GLuint setLighting(bool on, GLuint &pid);
    //======================
    void drawGrid();
    void genGrid();
    //======================
    void computeMatricesFromInputs();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    GLFWwindow *window;
    //===
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    //
    std::vector<float> grid;
};

#endif //GL3_0_WINDOW_H
