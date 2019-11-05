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
#include "../../glutils/shader/Texture.h"

glm::vec2 WindowDimensions;

ShaderProgram* colorShader;
ShaderProgram* texShader;
Texture* blueColor;
Texture* redColor;
GLuint blueTex;
GLuint redTex;

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

void draw()
{

}

void drawTransparent()
{

}

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
    glEnable(GL_BLEND);

    colorShader = new ShaderProgram(nullptr, "shaders/colorVert.glsl", "shaders/colorFrag.glsl");
    texShader = new ShaderProgram(nullptr, "shaders/materialVert.glsl", "shaders/materialFrag.glsl");

    sphere = new Sphere(0.5f, 10, 10);
    cube = new Cube();
    obb = new Cube();
    aobb = new Cube();
    Projection = glm::perspective(45.0f, WindowDimensions.x / WindowDimensions.y, 0.1f, 100.0f);
    cam = new Camera(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    unsigned char blue[] =
            {
                    static_cast<unsigned char>(0.0f * 255),
                    static_cast<unsigned char>(0.0f * 255),
                    static_cast<unsigned char>(1.0f * 255),
                    static_cast<unsigned char>(1.0f * 255)
            };
    unsigned char red[] =
            {
                    static_cast<unsigned char>(1.0f * 255),
                    static_cast<unsigned char>(0.0f * 255),
                    static_cast<unsigned char>(0.0f * 255),
                    static_cast<unsigned char>(1.0f * 255)
            };

    blueColor = new Texture(blue);
    redColor = new Texture(red);

    // custom bind func to ignore specular tex
    auto bindFunc = [](GLuint texID, GLuint)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texID);
            };
    blueColor->setBindFunc(bindFunc);
    redColor->setBindFunc(bindFunc);

    glGenTextures(1, &blueTex);
    glBindTexture(GL_TEXTURE_2D, blueTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blue);

    glGenTextures(1, &redTex);
    glBindTexture(GL_TEXTURE_2D, redTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, red);


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
    colorShader->use();

    colorShader->setMatrix4f("Projection", Projection);
    colorShader->setMatrix4f("View", cam->getViewMatrix());

    colorShader->setVec4f("Color", glm::vec4(1.0f));
    colorShader->setMatrix4f("Model", sphere->getTransformationMatrix());
    sphere->render();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    colorShader->setVec4f("Color", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    aobb->render();

    colorShader->setVec4f("Color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    obb->setLocalScale(sphere->getScale());
    obb->setLocalPosition(sphere->getPosition());
    colorShader->setMatrix4f("Model", obb->getTransformationMatrix());
    obb->render();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    texShader->use();
    cube->setLocalPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    texShader->setMatrix4f("VPMat", Projection*cam->getViewMatrix());
    texShader->setMatrix4f("MMat", cube->getTransformationMatrix());

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, blueTex);
    blueColor->bind();
    cube->render();

    glEnable(GL_BLEND);
    glDepthFunc(GL_EQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
//    glBindTexture(GL_TEXTURE_2D, redTex);
    redColor->bind();
    cube->render();
    glActiveTexture(0);
    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);
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
        selection->rotateLocal((lx - xpos) * -0.5f, glm::normalize(cam->getWorldForward()*cam->getOrientation()));
        selection->rotateLocal((ly - ypos) * -0.5f, cam->getWorldRight()*cam->getOrientation());
    }
    ly = ypos;
    lx = xpos;
}