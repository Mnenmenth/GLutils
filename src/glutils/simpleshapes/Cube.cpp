/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"

const std::array<GLfloat, Cube::NUM_INDICES> Cube::vertices =
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

std::array<GLuint, Cube::NUM_INDICES> Cube::indices;

const std::array<GLfloat, Cube::NUM_INDICES> Cube::normals =
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

    // Fill index array. (4*i) is the stride for each vertex group and (6*i) is the stride for each pair of triangles
    for(int i = 0; i < 6; i++)
    {
        indices[0+(6*i)] = 1+(4*i);
        indices[1+(6*i)] = 0+(4*i);
        indices[2+(6*i)] = 2+(4*i);
        indices[3+(6*i)] = 2+(4*i);
        indices[4+(6*i)] = 3+(4*i);
        indices[5+(6*i)] = 1+(4*i);
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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)sizeof(normals));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::render()
{
    // Draw cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}