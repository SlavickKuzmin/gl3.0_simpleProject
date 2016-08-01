#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShadersTool.h"
using namespace glm;
GLFWwindow *window;

int main()
{

    if(!glfwInit())
    {
        fprintf(stderr, "GLFWn init error!");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Tutorial 2 - red cube", NULL, NULL);

    if(window == NULL)
    {
        fprintf(stderr, "GL error, version not supported!");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW init error!");
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.f, 0.f, 0.4f, 0.f);

    //Create VBO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("/home/slavickkuzmin/lessons/gl3_0/simpleVertex.glsl",
                                   "/home/slavickkuzmin/lessons/gl3_0/simpleFragment.glsl");
    //
    static const GLfloat g_vertex_buffer_data[] = {
           -1.f,  -1.f,  0.f,
            1.f,  -1.f, 0.f,
            0.f,   1.f,  0.f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);





    //main cycle
    do
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programID);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0,        //Шейдери
                              3,        //Размер
                              GL_FLOAT, //Тип
                              GL_FALSE, //Указывает, что значения не нормализованы
                              0,        //Шаг
                              (void*)0  //Смещение масива в буффере
        );
        //Вывести треугольник
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);
    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}