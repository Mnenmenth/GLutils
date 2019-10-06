/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  *
  */

#include "ShaderProgram.h"

#define glUniform(type, value) glUniform ## type(glGetUniformLocation(m_ID, name), value)
#define glUniformMatrix(type, value) glUniformMatrix ## type(glGetUniformLocation(m_ID, name), 1, GL_FALSE, value)

const char* ShaderProgram::ShaderTypeNames[] = {"Program", "Vertex", "Fragment"};

ShaderProgram::ShaderProgram(const char *vertexPath, const char *fragmentPath) {
    std::string vertexSource;
    std::string fragmentSource;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // Read contents of vertex shader file
        vertexFile.open(vertexPath);
        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexSource = vertexStream.str();

        // Read contents of fragment shader file
        fragmentFile.open(fragmentPath);
        std::stringstream fragmentSteam;
        fragmentSteam << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentSource = fragmentSteam.str();

    } catch (std::ifstream::failure &e) {
        std::cout << "Unable to read shader file(s): " << e.what() << std::endl;
    }

    unsigned int vertex;
    const char* vSource = vertexSource.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vSource, nullptr);
    glCompileShader(vertex);
    CompileStatus(vertex, VERTEX);

    unsigned int fragment;
    const char* fSource = fragmentSource.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fSource, nullptr);
    glCompileShader(fragment);
    CompileStatus(fragment, FRAGMENT);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    CompileStatus(m_ID, PROGRAM);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

unsigned int ShaderProgram::getProgramID() {
    return m_ID;
}

void ShaderProgram::use() {
    glUseProgram(m_ID);
}

void ShaderProgram::setInt(const char *name, GLint value) {
    glUniform(1i, value);
}

void ShaderProgram::setFloat(const char *name, GLfloat value) {
    glUniform(1f, value);
}

void ShaderProgram::setMatrix4f(const char *name, const glm::mat4& value) {
    glUniformMatrix(4fv, &value[0][0]);
}


void ShaderProgram::CompileStatus(GLuint ID, ShaderType type) {
    GLint success;
    GLint logLength;
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength);
    GLchar infoLog[logLength];

    if(type == PROGRAM) { // Program Link Status
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(success) {
            std::cout << ShaderTypeNames[type] << " Linked Successfully " << std::endl;
        } else {
            glGetProgramInfoLog(ID, logLength, nullptr, infoLog);
            std::cout << ShaderTypeNames[type] << " Linking Error: \n" << infoLog << std::endl;
        }
    } else { // Shader Compile Status
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if(success) {
            std::cout << ShaderTypeNames[type] << " Compiled Successfully" << std::endl;
        } else {
            glGetShaderInfoLog(ID, logLength, nullptr, infoLog);
            std::cout << ShaderTypeNames[type] << " Shader Compilation Failed: \n" << infoLog << std::endl;
        }
    }
}