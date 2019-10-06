/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  *
  */

#ifndef GLUTILS_SHADERPROGRAM_H
#define GLUTILS_SHADERPROGRAM_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/mat4x4.hpp>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    unsigned int getProgramID();
    void use();
    void setInt(const char* name, GLint value);
    void setFloat(const char* name, GLfloat value);
    void setMatrix4f(const char* name, const glm::mat4& matrix);

private:
    GLuint m_ID;
    enum ShaderType {
        PROGRAM,
        VERTEX,
        FRAGMENT
    };
    static const char* ShaderTypeNames[];
    static void CompileStatus(GLuint ID, ShaderType type);

};


#endif //GLUTILS_SHADERPROGRAM_H
