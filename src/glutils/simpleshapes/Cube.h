/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#ifndef CS370_ASSIGN02_CUBE_H
#define CS370_ASSIGN02_CUBE_H

#include "../Renderable.h"
#include "../mesh/Vertex.h"
#include <array>

class Cube : public Renderable
{
public:
    Cube();
    void render() override;

private:
    static bool hasInit;
    static const std::vector<Vertex> vertices;
    static std::vector<GLuint> indices;

    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

    static void initStaticObj();
};


#endif //CS370_ASSIGN02_CUBE_H
