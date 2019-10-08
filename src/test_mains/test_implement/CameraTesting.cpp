#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <chrono>
#include "../../glutils/shader/ShaderProgram.h"
#include "../../glutils/camera/Camera.h"

glm::vec2 WindowDimensions;

ShaderProgram* shaders;
Camera* camera;

namespace SolidCube {
    GLuint VAO, VBO;
    const GLfloat vertices[] = {
            // Vertices                                    // Colors

            // Back Face
            0.5f,  0.5f, -0.5f, /* Back Top Right     */     0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, /* Back Bottom Right  */     0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, /* Back Bottom Left   */     0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, /* Back Bottom Left   */     0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, /* Back Top Left      */     0.0f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f, /* Back Top Right     */     0.0f, 0.0f, 1.0f,

            // Front Face
            0.5f, -0.5f,  0.5f, /* Front Bottom Right */     1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f, /* Front Top Right    */     1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, /* Front Top Left     */     1.0f, 0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, /* Front Top Left     */     1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, /* Front Bottom Left  */     1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f, /* Front Bottom Right */     1.0f, 0.0f, 0.0f,

            // Left Face
            -0.5f,  0.5f, -0.5f, /* Back Top Left      */     0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, /* Back Bottom Left   */     0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, /* Front Bottom Left  */     0.0f, 1.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, /* Front Top Left     */     0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, /* Back Top Left      */     0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, /* Front Bottom Left  */     0.0f, 1.0f, 0.0f,

            // Right Face
            0.5f,  0.5f, -0.5f, /* Back Top Right     */     1.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f, /* Front Top Right    */     1.0f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, /* Back Bottom Right  */     1.0f, 1.0f, 0.0f,

            0.5f, -0.5f, -0.5f, /* Back Bottom Right  */     1.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f, /* Front Top Right    */     1.0f, 1.0f, 0.0f,
            0.5f, -0.5f,  0.5f, /* Front Bottom Right */     1.0f, 1.0f, 0.0f,

            // Top Face
            0.5f,  0.5f,  0.5f, /* Front Top Right    */     1.0f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f, /* Back Top Right     */     1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, /* Front Top Left     */     1.0f, 0.0f, 1.0f,

            0.5f,  0.5f, -0.5f, /* Back Top Right     */     1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, /* Back Top Left      */     1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, /* Front Top Left     */     1.0f, 0.0f, 1.0f,

            // Bottom Face
            0.5f, -0.5f, -0.5f, /* Back Bottom Right  */     0.0f, 1.0f, 1.0f,
            0.5f, -0.5f,  0.5f, /* Front Bottom Right */     0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, /* Front Bottom Left  */     0.0f, 1.0f, 1.0f,

            -0.5f, -0.5f,  0.5f, /* Front Bottom Left  */     0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, /* Back Bottom Left   */     0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, /* Back Bottom Right  */     0.0f, 1.0f, 1.0f,

    };
}

bool Perspective = true;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void render();
int main() {

    WindowDimensions = glm::vec2(800, 800);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef OSX
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // OSX

    GLFWwindow* window = glfwCreateWindow(WindowDimensions.x, WindowDimensions.y, "GLutils Tests", nullptr, nullptr);
    if(window == nullptr) {
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD init failed" << std::endl;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetKeyCallback(window, keyCallback);

    glViewport(0, 0, WindowDimensions.x, WindowDimensions.y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    shaders = new ShaderProgram("../src/vertex.glsl", "../src/fragment.glsl");

    camera = new Camera(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0));

    glGenVertexArrays(1, &SolidCube::VAO);
    glGenBuffers(1, &SolidCube::VBO);

    glBindVertexArray(SolidCube::VAO);

    glBindBuffer(GL_ARRAY_BUFFER, SolidCube::VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SolidCube::vertices), SolidCube::vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /// Timer Variables
    auto lastTime = std::chrono::steady_clock::now();
    GLint fps = 60;

    while(!glfwWindowShouldClose(window)) {

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-lastTime).count();
        if(elapsed >= 1000/fps) {
            render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

///
/// FREE VS ORBIT CAMERA
///     FREE CAMERA HAS EYE POSITION OF 0 AND IS "LOOKING" 1.0f IN FRONT OF IT IN ANY DIRECTION
///
///     ORBIT CAMERA EYE POSITION IS INITIAL PLACEMENT AWAY FROM ORBIT TARGET. "LOOKING" *MUST* STAY AT 0
///
///     BOTH CAN BE MOVED AROUND IN SPACE BY TRANSLATING VIEW MATRIX BY A VECTOR
///     BOTH SHOULD HAVE UP AS THE WORLD UP AND LET THE ORIENTATION QUAT ROTATE THE CAMERA UP
///
///     view = lookAt(eye, at, up)
///     view = view * mat(Orientation)
///     view = translate(view, cameraPos)
///                             ^^ Camera Pos is the camera's position in the world. VERY different than eye position
///

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    shaders->use();

    /// Setup basic cameras
    glm::mat4 Projection;
    if(Perspective)
        Projection = glm::perspective(glm::radians(45.0f), (float)WindowDimensions.x / (float)WindowDimensions.y, 0.1f, 100.0f);
    else
        Projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -0.1f, 100.0f);

    shaders->setMatrix4f("Projection", Projection);

    shaders->setMatrix4f("View", camera->getViewMatrix());

    glBindVertexArray(SolidCube::VAO);

    /// Simple XYZ axis indicators
    glm::mat4 DebugModel = glm::mat4(1.0f);
    DebugModel = glm::translate(DebugModel, glm::vec3(4.0f, 0.0f, 0.0f));
    DebugModel = glm::scale(DebugModel, glm::vec3(8.0f, 0.1f, 0.1f));

    shaders->setMatrix4f("Model", DebugModel);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    DebugModel = glm::mat4(1.0f);
    DebugModel = glm::translate(DebugModel, glm::vec3(0.0f, 4.0f, 0.0f));
    DebugModel =  glm::scale(DebugModel, glm::vec3(0.1f, 8.0f, 0.1f));

    shaders->setMatrix4f("Model", DebugModel);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    DebugModel = glm::mat4(1.0f);
    DebugModel = glm::translate(DebugModel, glm::vec3(0.0f, 0.0f, 4.0f));
    DebugModel =  glm::scale(DebugModel, glm::vec3(0.1f, 0.1f, 8.0f));
    shaders->setMatrix4f("Model", DebugModel);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    /// Unit base
    glm::vec3 Pos(0.0f, 0.0f, 0.0f);
    glm::quat Orient = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 Size(1.0f, 1.0f, 1.0f);
    glm::mat4 Model(1.0f);
    Model = glm::translate(Model, Pos);
    Model = Model * glm::mat4_cast(Orient);
    Model = glm::scale(Model, Size);
    shaders->setMatrix4f("Model", Model);
    //glDrawArrays(GL_TRIANGLES, 0, 36);

    Pos = glm::vec3(5.0f, 2.0f, 1.0f);
    Orient = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Size = glm::vec3(1.0f, 1.0f, 1.0f);
    Model = glm::mat4(1.0f);
    Model = glm::translate(Model, Pos);
    Model = Model * glm::mat4_cast(Orient);
    Model = glm::scale(Model, Size);
    shaders->setMatrix4f("Model", Model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    Pos = glm::vec3(-10.0f, 6.0f, -3.0f);
    Orient = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Size = glm::vec3(1.0f, 1.0f, 1.0f);
    Model = glm::mat4(1.0f);
    Model = glm::translate(Model, Pos);
    Model = Model * glm::mat4_cast(Orient);
    Model = glm::scale(Model, Size);
    shaders->setMatrix4f("Model", Model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    Pos = glm::vec3(0.0f, 0.0f, -10.0f);
    Orient = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Size = glm::vec3(1.0f, 1.0f, 1.0f);
    Model = glm::mat4(1.0f);
    Model = glm::translate(Model, Pos);
    Model = Model * glm::mat4_cast(Orient);
    Model = glm::scale(Model, Size);
    shaders->setMatrix4f("Model", Model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    Pos = glm::vec3(1.0f, 10.0f, 0.0f);
    Orient = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Size = glm::vec3(1.0f, 1.0f, 1.0f);
    Model = glm::mat4(1.0f);
    Model = glm::translate(Model, Pos);
    Model = Model * glm::mat4_cast(Orient);
    Model = glm::scale(Model, Size);
    shaders->setMatrix4f("Model", Model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

float Speed = 2.0f;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->rotateLeft(Speed);
    }
    if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->rotateRight(Speed);
    }
    if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->rotateUp(Speed);
    }
    if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->rotateDown(Speed);
    }

    if(key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->rollLeft(Speed);
    }

    if(key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->rollRight(Speed);
    }

    if(key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->resetCamera();
    }

    if(key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->rotateWorldLeft(Speed);
    }

    if(key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->rotateWorldRight(Speed);
    }

    if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->moveForward(Speed);
    }
    if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->moveBackward(Speed);
    }
    if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->moveLeft(Speed);
    }
    if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->moveRight(Speed);
    }
    if(key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->moveUp(Speed);
    }
    if(key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->moveDown(Speed);
    }

    if(key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->moveWorldDown(Speed);
    }
    if(key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

        camera->moveWorldUp(Speed);
    }
    if(key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera->setCamPosition(glm::vec3(3.0f, 5.0f, 15.0f));
    }
    if(key == GLFW_KEY_P && action == GLFW_PRESS) {
        Perspective = !Perspective;
    }

}