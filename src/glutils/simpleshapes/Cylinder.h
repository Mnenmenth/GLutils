/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef CS370_ASSIGN02_CYLINDER_H
#define CS370_ASSIGN02_CYLINDER_H

#include "../Renderable.h"
#include "../mesh/Vertex.h"

/// Cylinder object. Can specify whether the top/bottom should be filled(capped) and how much of the cylinder is created
// Texture coordinates are probably very messy. However this isn't meant for complex stuff
class Cylinder : public Renderable
{
public:

    Cylinder(float height, float radius, int stacks, int slices, bool capped = true, float sweep = 360.0f);
    void render() override;

private:
    std::vector<Vertex>    vertices;
    std::vector<GLuint>     indices;
    void createCaps(float height, float radius, int slices, float sweep);

    GLuint VAO, VBO, EBO;
};


#endif //CS370_ASSIGN02_CYLINDER_H
