//
// Created by slavickkuzmin on 8/15/16.
//

#include "Window.h"
#include <vector>
//TODO fix it

Window::Window(const char *name, bool mode) {
    genGrid();//fix it
    if (!glfwInit()) {
        fprintf(stderr, "GLFWn init error!");
        //exit
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (mode) {
    window = glfwCreateWindow(1366, 768, name, glfwGetPrimaryMonitor(), NULL);
    }
    else
    {window = glfwCreateWindow(1024, 768, name, NULL, NULL);}
    if(window == NULL)
    {
        fprintf(stderr, "GL error, version not supported!");
        glfwTerminate();
        //exit
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        fprintf(stderr, "GLEW init error!");
        //exit
    }
//===========delete====
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    //
}

GLFWwindow* Window::getWindow() {
    return window;
}

glm::mat4 Window::getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 Window::getProjectionMatrix(){
    return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 0 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 0.f;
// Initial vertical angle : none
float verticalAngle = 3.1415f;//invert mouse 3.14
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;



void Window::computeMatricesFromInputs(){

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(1024/2 - xpos );
    verticalAngle   += mouseSpeed * float( 768/2 - ypos );

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross( right, direction );

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }

    float FoV = initialFoV;//- 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix       = glm::lookAt(
            position,           // Camera is here
            position+direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // For the next frame, the "last time" will be "now"
    // printf("h:%f v:%f\n", horizontalAngle, verticalAngle);
    lastTime = currentTime;
}

void Window::zBufferOnOff(bool on) {
    if(on){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

GLuint Window::setLighting(bool on, GLuint &pid) {
    if(on) {
        glUseProgram(pid);
        return glGetUniformLocation(pid, "LightPosition_worldspace");
    }
    else
    {
        return 0;
    }
}

Window::~Window() {
    glfwTerminate();
}

void Window::drawGrid() {
    GLuint vertexbuffer;

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(glm::vec3), &grid.at(0), GL_STATIC_DRAW);



    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0,        //Шейдери
                          3,        //Размер
                          GL_FLOAT, //Тип
                          GL_FALSE, //Указывает, что значения не нормализованы
                          0,        //Шаг
                          (void*)0  //Смещение масива в буффере
    );
    glDrawArrays(GL_LINES, 0, grid.size());
    glDisableVertexAttribArray(0);
    /*
    //==x
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(-linesLength, 0.f, 0.f);
    glVertex3f(linesLength, 0.f, 0.f);
    //===y
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.f, -linesLength, 0.f);
    glVertex3f(0.f, linesLength, 0.f);
    //===z
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.f, 0.f, -linesLength);
    glVertex3f(0.f, 0.f, linesLength);
    */
}

void Window::genGrid() {
    const float linesLength = 20.f;
    for(int i = -linesLength; i <= linesLength;i += 1.f) {
        grid.push_back(-linesLength); grid.push_back(0.f); grid.push_back(i);
        grid.push_back(linesLength);  grid.push_back(0.f); grid.push_back(i);
        grid.push_back(-i);           grid.push_back(0.f); grid.push_back(-linesLength);
        grid.push_back(-i);           grid.push_back(0.f); grid.push_back(linesLength);
    }
}
