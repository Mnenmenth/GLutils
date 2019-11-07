/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Cylinder.h"
// Vertices/normals/indices are generated basically the same as the UV sphere, just modified a bit

Cylinder::Cylinder(float height, float radius, int stacks, int slices, bool capped, float sweep) : Renderable({1.0f, 1.0f, 1.0f})
{
    /// Reserve space ahead of time for performance
    vertices.reserve(stacks*(slices+1));
    indices.reserve(vertices.capacity()*3);

    float stackSize = height / (float)(stacks-1);
    float sliceSize = glm::radians(sweep) / (float)slices;

    for(int stack = 0; stack < stacks; stack++)
    {
        float y = -height / 2.0f + (float)stack * stackSize;

        for(int slice = 0; slice <= slices; slice++)
        {
            float sliceAngle = (float)slice * sliceSize;
            glm::vec3 vertex(radius * glm::cos(sliceAngle), y, radius * glm::sin(sliceAngle));

            vertices.push_back(
                    {
                        vertex,
                        glm::normalize(vertex),
                        {
                                static_cast<float>(slice) / static_cast<float>(slices),
                                static_cast<float>(stack) / static_cast<float>(stacks)
                        }
                    }
                    );
        }
    }

    for(int i = 0; i < stacks - 1; i++)
    {
        int topLeft = i * (slices + 1);
        int bottomLeft = topLeft + slices + 1;

        for (int j = 0; j < slices; j++, topLeft++, bottomLeft++)
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
            indices.push_back(topLeft + 1);

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
            indices.push_back(topLeft + 1);
            indices.push_back(bottomLeft);
            indices.push_back(bottomLeft + 1);
        }
    }

    if(capped) createCaps(height, radius, slices, sweep);

    size_t arrSize = sizeof(GLfloat) * vertices.size();

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

void Cylinder::render()
{
    // Draw Cylinder
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Cylinder::createCaps(float height, float radius, int slices, float sweep)
{
    float sliceSize = glm::radians(sweep) / (float)slices;

    // Last index
    int index = vertices.size();

    // Center of the cap
    float capCenter = height/2.0f;

    // Center point for the cap
    glm::vec3 center(0.0f, -capCenter, 0.0f);

    vertices.push_back(
            {
                    center,
                    glm::normalize(center),
                    {0.5f, 0.5f}
            }
            );

    // Create vertices around the cap top
    for(int slice = 0; slice <= slices; slice++)
    {
        float sliceAngle = (float)slice * sliceSize;
        glm::vec3 vertex(radius * glm::cos(sliceAngle), -capCenter, radius * glm::sin(sliceAngle));

        vertices.push_back(
                {
                        vertex,
                        glm::normalize(vertex),
                        {glm::cos(sliceAngle), glm::sin(sliceAngle)}
                }
        );
    }

    // Index the cap top
    for (int j = 0; j < slices; j++)
    {
        indices.push_back(index);
        indices.push_back(index + j + 1);
        indices.push_back(index + j + 2);
    }

    // Same as above but reverse ordered indices
    index = vertices.size();

    center = glm::vec3(0.0f, capCenter, 0.0f);
    vertices.push_back(
            {
                    center,
                    glm::normalize(center),
                    {0.5f, 0.5f}
            }
    );

    for(int slice = 0; slice <= slices; slice++)
    {
        float sliceAngle = (float)slice * sliceSize;
        glm::vec3 vertex(radius * glm::cos(sliceAngle), capCenter, radius * glm::sin(sliceAngle));

        vertices.push_back(
                {
                        vertex,
                        glm::normalize(vertex),
                        {glm::cos(sliceAngle), glm::sin(sliceAngle)}
                }
        );
    }

    for (int j = 0; j < slices; j++)
    {
        indices.push_back(index + j + 2);
        indices.push_back(index + j + 1);
        indices.push_back(index);
    }
}