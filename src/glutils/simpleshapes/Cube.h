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
    static constexpr size_t NUM_VERTICES = 72;
    static constexpr size_t NUM_UVCOORDS = 48;
    static constexpr size_t NUM_INDICES = 36;

    Cube();
    void render() override;

private:
    static bool hasInit;
    static const std::array<GLfloat, NUM_VERTICES> vertices;
    static const std::array<GLfloat, NUM_VERTICES> normals;
    static std::array<GLfloat, NUM_UVCOORDS> uvcoords;
    static std::array<GLuint, NUM_INDICES> indices;

    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

    static void initStaticObj();
};


#endif //CS370_ASSIGN02_CUBE_H
