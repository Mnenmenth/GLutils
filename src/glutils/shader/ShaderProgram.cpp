/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  *
  * Modified from https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
  */

#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>

#define glUniform(type, value) glUniform ## type(glGetUniformLocation(m_ID, name), value)
#define glUniformVec(type, value) glUniform ## type(glGetUniformLocation(m_ID, name), 1, value)
#define glUniformMatrix(type, value) glUniformMatrix ## type(glGetUniformLocation(m_ID, name), 1, GL_FALSE, value)

const char* ShaderProgram::ShaderTypeNames[] = {"Program", "Vertex", "Fragment", "Geometry"};

ShaderProgram::ShaderProgram(const char* geomPath, const char *vertexPath, const char *fragmentPath) {
    std::string geomSource;
    std::string vertexSource;
    std::string fragmentSource;

    std::ifstream geomFile;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    geomFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // Read contents of geom shader file
        if(geomPath != nullptr)
        {
            geomFile.open(geomPath);
            std::stringstream geomStream;
            geomStream << geomFile.rdbuf();
            geomFile.close();
            geomSource = geomStream.str();
        }

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

    unsigned int geom;
    if(geomPath != nullptr)
    {
        const char* gSource = geomSource.c_str();
        geom = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geom, 1, &gSource, nullptr);
        glCompileShader(geom);
        CompileStatus(geom, GEOMETRY);
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
    if(geomPath != nullptr) glAttachShader(m_ID, geom);
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

void ShaderProgram::setUInt(const char *name, GLuint value)
{
    glUniform(1ui, value);
}

void ShaderProgram::setFloat(const char *name, GLfloat value) {
    glUniform(1f, value);
}

void ShaderProgram::setVec3f(const char *name, const glm::vec3 &value) {
    glUniformVec(3fv, &value[0]);
}

void ShaderProgram::setVec4f(const char *name, const glm::vec4 &value)
{
    glUniformVec(4fv, &value[0]);
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