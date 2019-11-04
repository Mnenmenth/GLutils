/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#ifndef CS370_ASSIGN02_OCTAHEDRON_H
#define CS370_ASSIGN02_OCTAHEDRON_H

#include "../Renderable.h"
#include <array>

class Octahedron : public Renderable
{
public:
    static const int VERT_ARR_LEN = 72;

    Octahedron();
    void render() override;

private:
    static bool hasInit;
    static const std::array<GLfloat, VERT_ARR_LEN> vertices;
    static std::array<GLfloat, VERT_ARR_LEN> normals;

    static GLuint VAO;
    static GLuint VBO;

    static void initStaticObj();
};


#endif //CS370_ASSIGN02_OCTAHEDRON_H
