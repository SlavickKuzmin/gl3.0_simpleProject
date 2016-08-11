#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShadersTool.h"
#include "texture.h"
#include "controls.h"
#include "model.h"

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

    //version
    /*
    printf("OpenGL version: %s\n",glGetString(GL_VERSION));
    printf("GLSL version: %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("Vendor: %s\n",glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    */
     //=======

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
    //
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    //
    glClearColor(0.f, 0.f, 0.4f, 0.f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    //Create VBO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("/home/slavickkuzmin/lessons/gl3_0/simpleVertex.glsl",
                                   "/home/slavickkuzmin/lessons/gl3_0/simpleFragment.glsl");





    //Получити хендл змінної в шейдері тільки один рах при ініціалізації
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

    //Проекційна матриця: 45 градусів поле обзора, 4:3 відношення сторін, діапазон 0.1 юніт -- 100 юнітів
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
    //маттриця камери
    glm::mat4 View = glm::lookAt(glm::vec3(4,3,-3),//Камера знаходиться в ствітових координатах (4.3.3)
                                 glm::vec3(0,0,0),//І направлена в начало координат
                                 glm::vec3(0,1,0) //Голова знаходиться зверху
    );
    //Матриця моделі: одинична матриця(модель знаходиться в початку координат)
    glm::mat4 Model = glm::mat4(1.f); //Індивідуально для кожної моделі
    //Ітогова матриця проекції являється результатом множення цих матриць
    glm::mat4 MVP = Projection * View * Model;
    //Textures
    //GLuint Texture = loadBMP_custom("/home/slavickkuzmin/ogl-OpenGL-tutorial_0015_33/tutorial05_textured_cube/uvtemplate.bmp");

    const char *tname = "/home/slavickkuzmin/lessons/SGL/les1/texture.bmp";
    //const char *tname = "/home/slavickkuzmin/Downloads/m3d/Old House 2/Old House Texture/House.bmp";

    GLuint Texture = loadBMP_custom(tname);
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    const char *name = "/home/slavickkuzmin/lessons/SGL/les1/african_head.obj";
    //const char *name = "/home/slavickkuzmin/Downloads/m3d/Old House 2/House.obj";

    LoadOBJModel *model = new LoadOBJModel(name);

    //передати наші трансформації в тікущий шейдер
    //для кожної моделі, яку ми вводимо в MVP буде різним

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, model->vertices->size() * sizeof(glm::vec3), &model->vertices->at(0), GL_STATIC_DRAW);
    //GLuint colorbuffer;
    //glGenBuffers(1, &colorbuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    //texture
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, model->uvs->size() * sizeof(glm::vec2), &model->uvs->at(0), GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, model->normals->size() * sizeof(glm::vec3), &model->normals->at(0), GL_STATIC_DRAW);

    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");


    //main cycle
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        glUseProgram(programID);
        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
        //
        glm::vec3 lightPos = glm::vec3(20,20,10);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);
        //
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0,        //Шейдери
                              3,        //Размер
                              GL_FLOAT, //Тип
                              GL_FALSE, //Указывает, что значения не нормализованы
                              0,        //Шаг
                              (void*)0  //Смещение масива в буффере
        );
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );
        //Вывести треугольник
        glDrawArrays(GL_TRIANGLES, 0, model->vertices->size());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);
    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    //glDeleteBuffers(1, &colorbuffer);
    glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}