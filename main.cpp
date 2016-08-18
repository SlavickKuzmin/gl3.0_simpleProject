#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShadersTool.h"
#include "model.h"

#include "Window.h"
#include "Text2D.h"

using namespace glm;


int main()
{
    Window wnd("Test Window", false);

    wnd.setBackground(1.f, 1.f, 1.f, 0.f);
    wnd.zBufferOnOff(true);
    Shaders shader("/home/slavickkuzmin/lessons/gl3_0/simpleVertex.glsl",
                                   "/home/slavickkuzmin/lessons/gl3_0/simpleFragment.glsl");
    GLuint LightID = wnd.setLighting(true, shader.ProgramID);

    //=============================================================================================
    //const char *name = "/home/slavickkuzmin/ogldev-source/Content/phoenix_ugv.md2";
    //const char *name = "/home/slavickkuzmin/Downloads/temp/Darth_Sidious/ds.obj";
    //const char *name = "/home/slavickkuzmin/Downloads/temp/Chinese house/ch.obj";
    //const char *name = "/home/slavickkuzmin/Downloads/temp/ssjbcgosczr4-R0 1/RE 0 Main Hall/mh.obj";//cool
    const char *name = "/home/slavickkuzmin/Downloads/temp/1y3ycdp6e3ao-FH/RE SM front hall/smh.obj";
    //const char *name = "/home/slavickkuzmin/Downloads/m3d/Old House 2/House.obj";
    //const char *name = "/home/slavickkuzmin/Downloads/m3d/untitled.obj";
    //LoadOBJModel *model = new LoadOBJModel(name, shader.ProgramID);

    Mesh* m_pMesh;
    m_pMesh = new Mesh(shader.ProgramID, 2.2f);
    m_pMesh->LoadMesh(name);

    //LoadOBJModel *model = new LoadOBJModel(name, shader.ProgramID);


    //const char *tname = "/home/slavickkuzmin/lessons/SGL/les1/texture.bmp";
    //const char *tname = "/home/slavickkuzmin/Downloads/m3d/Old House 2/Old House Texture/House.bmp";
    //const char *tname = "/home/slavickkuzmin/Downloads/m3d/re4 village/8aa13b67.dds";
    //model->loadTextureBMP(tname);
    //model->loadTextureDDS(tname);

    //==========
    glm::vec3 lightPos = glm::vec3(0,20,0);
    //main cycle
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 MVP;

    initText2D( "/home/slavickkuzmin/lessons/gl3_0/Holstein.DDS" );
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    float FPS = 0.f;
    char text[256];
    do
    {
        //
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
            // printf and reset
            //printf("%f ms/frame\n", 1000.0/double(nbFrames));
            FPS = 1000.0/double(nbFrames);
            nbFrames = 0;
            lastTime += 1.0;
        }
        //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        glUseProgram(shader.ProgramID);
        // Compute the MVP matrix from keyboard and mouse input
        wnd.computeMatricesFromInputs();
        ProjectionMatrix = wnd.getProjectionMatrix();
        ViewMatrix = wnd.getViewMatrix();
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(shader.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(shader.ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(shader.ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
        //

        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        //
        wnd.drawGrid();
        glUseProgram(shader.ProgramID);

        m_pMesh->Render();

        //
        sprintf(text,"%.2f FPS", FPS );
        printText2D(text, 10, 550, 20);
        //

        glfwSwapBuffers(wnd.getWindow());
        glfwPollEvents();

    } while(glfwGetKey(wnd.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(wnd.getWindow()) == 0);
    cleanupText2D();
}