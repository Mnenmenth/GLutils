/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"

const std::array<GLfloat, Cube::NUM_VERTICES> Cube::vertices =
        {
                // Back Face
                  0.5f,  0.5f, -0.5f, /* Top Left       0*/
                 -0.5f,  0.5f, -0.5f, /* Top Right      1*/
                  0.5f, -0.5f, -0.5f, /* Bottom Left    2*/
                 -0.5f, -0.5f, -0.5f, /* Bottom Right   3*/

                // Front Face
                -0.5f,  0.5f,  0.5f, /* Top Left        4*/
                 0.5f,  0.5f,  0.5f, /* Top Right       5*/
                -0.5f, -0.5f,  0.5f, /* Bottom Left     6*/
                 0.5f, -0.5f,  0.5f, /* Bottom Right    7*/

                // Left Face
                -0.5f,  0.5f, -0.5f, /* Top Left        */
                -0.5f,  0.5f,  0.5f, /* Top Right       */
                -0.5f, -0.5f, -0.5f, /* Bottom Left     */
                -0.5f, -0.5f,  0.5f, /* Bottom Right    */

                // Right Face
                 0.5f,  0.5f,  0.5f, /* Top Left        */
                 0.5f,  0.5f, -0.5f, /* Top Right       */
                 0.5f, -0.5f,  0.5f, /* Bottom Left     */
                 0.5f, -0.5f, -0.5f, /* Bottom Right    */

                // Top Face
                -0.5f,  0.5f, -0.5f, /* Top Left        */
                 0.5f,  0.5f, -0.5f, /* Top Right       */
                -0.5f,  0.5f,  0.5f, /* Bottom Left     */
                 0.5f,  0.5f,  0.5f, /* Bottom Right    */

                // Bottom Face
                -0.5f, -0.5f,  0.5f, /* Top Left        */
                 0.5f, -0.5f,  0.5f, /* Top Right       */
                -0.5f, -0.5f, -0.5f, /* Bottom Left     */
                 0.5f, -0.5f, -0.5f, /* Bottom Right    */

        };

const std::array<GLfloat, Cube::NUM_VERTICES> Cube::normals =
        {
                // Back Face
                 0.0f,  0.0f, -1.0f,   /* Top Left        0*/
                 0.0f,  0.0f, -1.0f,   /* Top Right       1*/
                 0.0f,  0.0f, -1.0f,   /* Bottom Left     2*/
                 0.0f,  0.0f, -1.0f,   /* Bottom Right    3*/
                // Front Face
                 0.0f,  0.0f,  1.0f,   /* Top Left        */
                 0.0f,  0.0f,  1.0f,   /* Top Right       */
                 0.0f,  0.0f,  1.0f,   /* Bottom Left     */
                 0.0f,  0.0f,  1.0f,   /* Bottom Right    */
                // Left Face
                -1.0f,  0.0f,  0.0f,   /* Top Left        */
                -1.0f,  0.0f,  0.0f,   /* Top Right       */
                -1.0f,  0.0f,  0.0f,   /* Bottom Left     */
                -1.0f,  0.0f,  0.0f,   /* Bottom Right    */
                // Right Face
                 1.0f,  0.0f,  0.0f,   /* Top Left        */
                 1.0f,  0.0f,  0.0f,   /* Top Right       */
                 1.0f,  0.0f,  0.0f,   /* Bottom Left     */
                 1.0f,  0.0f,  0.0f,   /* Bottom Right    */
                // Top Face
                 0.0f,  1.0f,  0.0f,   /* Top Left        */
                 0.0f,  1.0f,  0.0f,   /* Top Right       */
                 0.0f,  1.0f,  0.0f,   /* Bottom Left     */
                 0.0f,  1.0f,  0.0f,   /* Bottom Right    */
                // Bottom Face
                 0.0f, -1.0f,  0.0f,   /* Top Left        */
                 0.0f, -1.0f,  0.0f,   /* Top Right       */
                 0.0f, -1.0f,  0.0f,   /* Bottom Left     */
                 0.0f, -1.0f,  0.0f,   /* Bottom Right    */
        };

std::array<GLfloat, Cube::NUM_UVCOORDS> Cube::uvcoords;

std::array<GLuint, Cube::NUM_INDICES> Cube::indices;

bool Cube::hasInit = false;
GLuint Cube::VAO;
GLuint Cube::VBO;
GLuint Cube::EBO;

Cube::Cube()
{
    initStaticObj();
}

void Cube::initStaticObj()
{
    if(hasInit) return;
    hasInit = true;

    // Fill uv coord array with same values for each face
    for(int i = 0; i < 8; i++)
    {
        // Stride for UV coords for each face. (2 coords * 4 vertices)
        int blockStride = (8*i);
        // Top left
        uvcoords[0+blockStride] = 0;
        uvcoords[1+blockStride] = 1;
        
        // Top Right
        uvcoords[2+blockStride] = 1;
        uvcoords[3+blockStride] = 1;

        // Bottom Left
        uvcoords[4+blockStride] = 0;
        uvcoords[5+blockStride] = 0;

        // Bottom Right
        uvcoords[6+blockStride] = 1;
        uvcoords[7+blockStride] = 0;
    }

    // Fill index array.
    for(int i = 0; i < 6; i++)
    {
        // Stride for pair of triangles (2 triangles * 3 vertices = 6)
        int blockStride = (6*i);
        // Stride for vertex-index group (1 index per vertex * 4 vertices for face = 4)
        int groupStride = (4*i);
        indices[0+blockStride] = 1+groupStride;
        indices[1+blockStride] = 0+groupStride;
        indices[2+blockStride] = 2+groupStride;
        indices[3+blockStride] = 2+groupStride;
        indices[4+blockStride] = 3+groupStride;
        indices[5+blockStride] = 1+groupStride;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Allocate size for buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), nullptr, GL_STATIC_DRAW);
    // Add vertices and normals to buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), &normals);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    // Set layout positions for shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<void*>(sizeof(normals)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), reinterpret_cast<void*>(sizeof(normals)+sizeof(uvcoords)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::render()
{
    // Draw cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}