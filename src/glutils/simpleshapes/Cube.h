/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#ifndef CS370_ASSIGN02_CUBE_H
#define CS370_ASSIGN02_CUBE_H

#include "../Renderable.h"
#include <array>

class Cube : public Renderable
{
public:
    static const int NUM_INDICES = 72;

    Cube();
    void render() override;

private:
    static bool hasInit;
    static const std::array<GLfloat, NUM_INDICES> vertices;
    static const std::array<GLfloat, NUM_INDICES> normals;
    static std::array<GLuint, NUM_INDICES> indices;

    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

    static void initStaticObj();
};


#endif //CS370_ASSIGN02_CUBE_H
