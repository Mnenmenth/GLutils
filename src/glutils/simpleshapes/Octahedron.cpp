/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#include <glm/gtc/type_ptr.hpp>
#include "Octahedron.h"

const std::array<GLfloat, Octahedron::VERT_ARR_LEN> Octahedron::vertices =
        {
             0.0f,  0.5f,  0.0f,
            -0.5f,  0.0f,  0.5f,
             0.5f,  0.0f,  0.5f,

             0.0f,  0.5f,  0.0f,
            -0.5f,  0.0f, -0.5f,
            -0.5f,  0.0f,  0.5f,

             0.0f,  0.5f,  0.0f,
             0.5f,  0.0f,  -0.5f,
            -0.5f,  0.0f,  -0.5f,

             0.0f,  0.5f,  0.0f,
             0.5f,  0.0f,  0.5f,
             0.5f,  0.0f, -0.5f,

             0.0f,  -0.5f,  0.0f,
             0.5f,   0.0f,  0.5f,
            -0.5f,   0.0f,  0.5f,

             0.0f,  -0.5f,  0.0f,
            -0.5f,   0.0f,  0.5f,
            -0.5f,   0.0f, -0.5f,

             0.0f,  -0.5f,  0.0f,
            -0.5f,   0.0f, -0.5f,
             0.5f,   0.0f, -0.5f,

             0.0f,  -0.5f,  0.0f,
             0.5f,   0.0f, -0.5f,
             0.5f,   0.0f,  0.5f,
        };

std::array<GLfloat, Octahedron::VERT_ARR_LEN> Octahedron::normals;

bool Octahedron::hasInit = false;
GLuint Octahedron::VAO;
GLuint Octahedron::VBO;

Octahedron::Octahedron()
{
    initStaticObj();
}
void Octahedron::initStaticObj()
{
    if(hasInit) return;
    hasInit = true;

    // Process in chunks of 3 vertices
    for(int i = 0; i < VERT_ARR_LEN/9; i++)
    {
        // Number of coordinates in each chunk
        int stride = 9*i;
        // Tip of the face
        glm::vec3 tip = glm::vec3(vertices[0+stride], vertices[1+stride], vertices[2+stride]);
        // Side of face
        glm::vec3 U = glm::vec3(vertices[3+stride], vertices[4+stride], vertices[5+stride]) - tip;
        // Opposite side of face
        glm::vec3 V = glm::vec3(vertices[6+stride], vertices[7+stride], vertices[8+stride]) - tip;
        // Normal vector perpendicular to face
        glm::vec3 normal = glm::normalize(glm::cross(U, V));

        // Fill corresponding chunk in normals array with normal vertex
        for(int j = 0; j < 3; j++)
        {
            normals[0+stride+(3*j)] = normal.x;
            normals[1+stride+(3*j)] = normal.y;
            normals[2+stride+(3*j)] = normal.z;
        }

    }

    // Create IDs
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Allocate size for buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), nullptr, GL_DYNAMIC_DRAW);
    // Add vertices and normals to buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), &normals);

    // Set layout positions for shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)sizeof(vertices));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Octahedron::render()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    glBindVertexArray(0);
}