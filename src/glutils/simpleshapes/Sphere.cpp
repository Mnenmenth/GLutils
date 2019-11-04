/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#include <glm/gtc/type_ptr.hpp>
#include "Sphere.h"
// http://www.songho.ca/opengl/gl_sphere.html was referenced for generating the sphere vertices/indices

Sphere::Sphere(float radius, int stacks, int slices)
{
    //! Stacks & Slices min count check is purposefully omitted because funny/interesting
    //! things can be made from low amounts of slices and/or stacks

    // Expand vectors to needed size in advance so time isn't wasted on resizing during calculations
    vertices.reserve(stacks*(slices+1)*3);
    normals.reserve(vertices.capacity());
    indices.reserve(vertices.capacity());

    /// Calculate vertices and normals of corners for each face of normal. aka slice-stack intersects
    // Size of a single slice and single stack in sphere
    float sliceSize = glm::radians(360.0f) / (float)slices;
    float stackSize = glm::radians(180.0f) / (float)stacks;

    for(int i = 0; i <= stacks; i++)
    {
        // Angle of stack along sphere radius (vertically)
        float stackAngle = glm::radians(90.0f) - ((float)i * stackSize);
        // Base vertex of current stack
        float xy = radius * glm::cos(stackAngle);
        float z = radius * glm::sin(stackAngle);

        for(int j = 0; j <= slices; j++)
        {
            // Angle of slice around entire sphere (horizontally)
            float sliceAngle = (float)j * sliceSize;

            // Vertex of where the current stack and slice intersect
            glm::vec3 vertex(xy * glm::cos(sliceAngle), xy * glm::sin(sliceAngle), z);

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            // Normal for vertex
            glm::vec3 normal = glm::normalize(vertex);

            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);

        }
    }

    /// Get the indices needed to the triangle(s) for each stack face
    for(int i = 0; i < stacks; i++)
    {
        // Top & Bottom left of the current stack face
        int topLeft = i * (slices + 1);
        int bottomLeft = topLeft + slices + 1;

        for(int j = 0; j < slices; j++, topLeft++, bottomLeft++)
        {
            // Top & Bottom stacks only need one triangle per face since
            // "top left" and "top right" are both polar north/south

            // If this isn't the first stack
            if(i != 0)
            {
                // TL       TR
                // *--------*
                // |\       |
                // | \      |
                // |  \     |
                // |   \    |
                // |    \   |
                // |     \  |
                // |      \ |
                // *--------*
                // BL       BR
                // Top Left -> Bottom Left -> Top Right
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topLeft+1);
            }

            // If this isn't the last stack
            if(i != stacks-1)
            {
                // TL       TR
                // *--------*
                // |       /|
                // |      / |
                // |     /  |
                // |    /   |
                // |   /    |
                // |  /     |
                // | /      |
                // *--------*
                // BL       BR
                // Top Right -> Bottom Left -> Bottom Right
                indices.push_back(topLeft+1);
                indices.push_back(bottomLeft);
                indices.push_back(bottomLeft+1);
            }
        }
    }

    // sizeof() equivalent for vertices underlying array since
    // sizeof(vertices) wouldn't give the value we want
    size_t arrSize = sizeof(GLfloat) * vertices.size();

    // Create IDs
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Allocate size for buffer
    glBufferData(GL_ARRAY_BUFFER, arrSize*2, nullptr, GL_STATIC_DRAW);
    // Add vertices, normals, and colors to buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, arrSize, vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, arrSize, arrSize, normals.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Buffer indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Set layout positions for shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)arrSize);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::render()
{
    // Draw sphere
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}