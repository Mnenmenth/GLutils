/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <chrono>
#include "../../glutils/shader/ShaderProgram.h"
#include "../../glutils/simpleshapes/Cube.h"
#include "../../glutils/camera/Camera.h"
#include "../../glutils/simpleshapes/Sphere.h"

glm::vec2 WindowDimensions;

ShaderProgram* shaders;

Sphere* sphere;
Cube* cube;
Cube* obb;
Cube* aobb;
Camera* cam;
glm::mat4 Projection;
// ray detection modified from opengl-tutorial.com
bool obbIntersection(glm::vec3 origin, glm::vec3 direction, glm::vec3 aabb_min, glm::vec3 aabb_max, glm::mat4 Model, float& intersection_dist);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseMoveCallback(GLFWwindow*, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow*, int button, int action, int mods);
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
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    glViewport(0, 0, WindowDimensions.x, WindowDimensions.y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    shaders = new ShaderProgram(nullptr, "../src/vertex.glsl", "../src/fragment.glsl");

    sphere = new Sphere(0.5f, 10, 10);
    cube = new Cube();
    obb = new Cube();
    aobb = new Cube();
    Projection = glm::perspective(45.0f, WindowDimensions.x / WindowDimensions.y, 0.1f, 100.0f);
    cam = new Camera(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


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

void render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    shaders->use();

    shaders->setMatrix4f("Projection", Projection);
    shaders->setMatrix4f("View", cam->getViewMatrix());

    shaders->setVec4f("Color", glm::vec4(1.0f));
    shaders->setMatrix4f("Model", sphere->getTransformationMatrix());
    sphere->render();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shaders->setVec4f("Color", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    aobb->render();

    shaders->setVec4f("Color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    obb->setLocalScale(sphere->getScale());
    obb->setLocalPosition(sphere->getPosition());
    shaders->setMatrix4f("Model", obb->getTransformationMatrix());
    obb->render();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shaders->setVec4f("Color", glm::vec4(1.0f));
    cube->setLocalPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    shaders->setMatrix4f("Model", cube->getTransformationMatrix());
    cube->render();
}

bool obbIntersection(glm::vec3 origin, glm::vec3 direction, glm::vec3 aabb_min, glm::vec3 aabb_max, glm::mat4 Model, float& intersection_dist)
{
    float tMin = 0.0f;
    float tMax = 100000.0f;

    glm::vec3 obbWorldSpace(Model[3]);
    glm::vec3 delta = obbWorldSpace - origin;

    for(int i = 0; i < 3; i++)
    {
        glm::vec3 axis(Model[i]);
        float e = glm::dot(axis, delta);
        float f = glm::dot(direction, axis);

        if(fabs(f) > 0.001f)
        {
            float t1 = (e+aabb_min[i])/f;
            float t2 = (e+aabb_max[i])/f;

            if(t1 > t2)
            {
                float w = t1;
                t1 = t2;
                t2 = w;
            }

            if(t2 < tMax)
                tMax = t2;

            if(t1 > tMin)
                tMin = t1;

            if(tMax < tMin)
                return false;
        }
        else
        {
            if(-e+aabb_min[i] > 0.0f || -e+aabb_max[i] < 0.0f)
                return false;
        }
    }

    intersection_dist = tMin;
    return true;

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if(key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, true);
    }

}

Renderable* selection;
double lx, ly = 0.0;
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        float dist;
        glm::vec3 min(-0.5f);
        glm::vec3 max(0.5f);

        glm::vec3 origin;
        glm::vec3 dir;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lx = xpos;
        ly = ypos;

        glm::vec4 rayStart((xpos/(float)WindowDimensions.x - 0.5f) * 2.0f, (ypos/(float)WindowDimensions.y - 0.5f) * 2.0f, -1.0f, 1.0f);
        glm::vec4 rayEnd((xpos/(float)WindowDimensions.x - 0.5f) * 2.0f, (ypos/(float)WindowDimensions.y - 0.5f) * 2.0f, 0.0f, 1.0f);

        glm::mat4 M = glm::inverse(Projection * cam->getViewMatrix());
        glm::vec4 rayStartWorld = M * rayStart;
        rayStartWorld /= rayStartWorld.w;
        glm::vec4 rayEndWorld = M * rayEnd;
        rayEndWorld /= rayEndWorld.w;

        glm::vec3 rayDirWorld(rayEndWorld - rayStartWorld);
        rayDirWorld = glm::normalize(rayDirWorld);

        origin = glm::vec3(rayStartWorld);
        dir = rayDirWorld;

        bool intersection = obbIntersection(origin, dir, min, max, sphere->getTransformationMatrix(), dist);
        if(intersection)
        {
            selection = sphere;
        }
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        selection = nullptr;
    }
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        cam->rotateWorldYaw((WindowDimensions.x/2.0f - xpos) * -0.2f);
        cam->rotatePitch((WindowDimensions.y/2.0f - ypos) * -0.2f);
        glfwSetCursorPos(window, WindowDimensions.x/2.0f, WindowDimensions.y/2.0f);
    }

    if(selection != nullptr)
    {
        selection->rotateLocal((ly - ypos) * -0.5f, glm::normalize(cam->getWorldRight()*cam->getOrientation()));
        selection->rotateLocal((lx - xpos) * -0.5f, glm::normalize(cam->getWorldForward()*cam->getOrientation()));
    }
    ly = ypos;
    lx = xpos;
}