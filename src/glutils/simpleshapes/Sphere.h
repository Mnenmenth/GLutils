/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#ifndef CS370_ASSIGN02_SPHERE_H
#define CS370_ASSIGN02_SPHERE_H

#include <array>
#include "../Renderable.h"
#include "../mesh/Vertex.h"

/// Simple uniform color UV sphere
class Sphere : public Renderable
{
public:
    Sphere(float radius, int stacks, int slices);
    void render() override;

private:
    std::vector<Vertex>     vertices;
    std::vector<GLuint>     indices;

    GLuint VAO, VBO, EBO;
};


#endif //CS370_ASSIGN02_SPHERE_H
