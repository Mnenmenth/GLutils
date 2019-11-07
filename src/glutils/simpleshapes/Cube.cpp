/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"

const std::vector<Vertex> Cube::vertices =
        {
            // Back Face
            {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0, 1}}, /* Top Left     */
            {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1, 1}}, /* Top Right    */
            {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0, 0}}, /* Bottom Left  */
            {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1, 0}}, /* Bottom Right */

            // Front Face
            {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0, 1}}, /* Top Left     */
            {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1, 1}}, /* Top Right    */
            {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0, 0}}, /* Bottom Left  */
            {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1, 0}}, /* Bottom Right */

            // Left Face
            {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0, 1}}, /* Top Left     */
            {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1, 1}}, /* Top Right    */
            {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0, 0}}, /* Bottom Left  */
            {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1, 0}}, /* Bottom Right */

            // Right Face
            {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0, 1}}, /* Top Left     */
            {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1, 1}}, /* Top Right    */
            {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0, 0}}, /* Bottom Left  */
            {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1, 0}}, /* Bottom Right */

            // Top Face
            {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0, 1}}, /* Top Left     */
            {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1, 1}}, /* Top Right    */
            {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0, 0}}, /* Bottom Left  */
            {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1, 0}}, /* Bottom Right */

            // Bottom Face
            {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0, 1}}, /* Top Left     */
            {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1, 1}}, /* Top Right    */
            {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0, 0}}, /* Bottom Left  */
            {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1, 0}}, /* Bottom Right */
        };


std::vector<GLuint> Cube::indices;

bool Cube::hasInit = false;
GLuint Cube::VAO;
GLuint Cube::VBO;
GLuint Cube::EBO;

Cube::Cube() : Renderable({1.0f, 1.0f, 1.0f})
{
    initStaticObj();
}

void Cube::initStaticObj()
{
    if(hasInit) return;
    hasInit = true;

    // Reserve memory ahead of time since size is already known
        // # of vertices (vertex::v = vec3, hence size()*3) / 2 triangles per face
    indices.reserve((vertices.size()*3) / 2);

    // Fill index array.
    for(int i = 0; i < 6; i++)
    {
        // Stride for vertex-index group (1 index per vertex * 4 vertices for face = 4)
        int groupStride = (4*i);
        indices.push_back(1+groupStride);
        indices.push_back(0+groupStride);
        indices.push_back(2+groupStride);
        indices.push_back(2+groupStride);
        indices.push_back(3+groupStride);
        indices.push_back(1+groupStride);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Buffer vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Buffer indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(), indices.data(), GL_STATIC_DRAW);

    // Set layout positions for shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, n)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, t)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::render()
{
    // Draw cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}