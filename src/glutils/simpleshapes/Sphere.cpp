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
    vertices.reserve(stacks * (slices + 1));
    indices.reserve(vertices.capacity()*3);

    /// Calculate vertices and normals of corners for each face of normal. aka slice-stack intersects
    // Size of a single slice and single stack in sphere
    float sliceSize = glm::radians(360.0f) / static_cast<float>(slices);
    float stackSize = glm::radians(180.0f) / static_cast<float>(stacks);

    for(int i = 0; i <= stacks; i++)
    {
        // Angle of stack along sphere radius (vertically)
        float stackAngle = glm::radians(90.0f) - (static_cast<float>(i) * stackSize);
        // Base vertex of current stack
        float xy = radius * glm::cos(stackAngle);
        float z = radius * glm::sin(stackAngle);

        for(int j = 0; j <= slices; j++)
        {
            // Angle of slice around entire sphere (horizontally)
            float sliceAngle = static_cast<float>(j) * sliceSize;

            // Vertex of where the current stack and slice intersect
            glm::vec3 vertex(xy * glm::cos(sliceAngle), xy * glm::sin(sliceAngle), z);

            vertices.push_back(
                    {
                        vertex,
                        glm::normalize(vertex),
                        {
                            static_cast<float>(j) / static_cast<float>(slices),
                            static_cast<float>(i) / static_cast<float>(stacks)
                        }
                    });

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

    // Create IDs
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Buffer vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Buffer indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Set layout positions for shaders

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, n)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, t)));
    glEnableVertexAttribArray(2);

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