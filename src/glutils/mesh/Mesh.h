/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef GLUTILS_MESH_H
#define GLUTILS_MESH_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include "../shader/Texture.h"

class Mesh
{
public:
    Mesh(std::string& objFile);
    Mesh(std::vector<GLfloat> &vertices, std::vector<GLfloat> &normals, std::vector<GLfloat> &uvcoords);

private:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvcoords;
    std::vector<GLuint> indices;
    //Texture texture;
};


#endif //GLUTILS_MESH_H
