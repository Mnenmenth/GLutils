/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include <iostream>
#include "Texture.h"

void Texture::createTexture_default(GLuint* texID, GLuint width, GLuint height, unsigned char data[])
{
    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_2D, *texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

}

void Texture::bind_default(GLuint texID, GLuint)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::bind_specDefault(GLuint texID, GLuint specTexID)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specTexID);
}

Texture::Texture(GLuint width, GLuint height, unsigned char data[], CreateTexture func) : m_Width(width), m_Height(height)
{
    func(&m_TexID, m_Width, m_Height, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_BindFunc = bind_default;
}

Texture::Texture(
        GLuint width, GLuint height,
        unsigned char data[],
        unsigned char specularData[],
        CreateTexture dataFunc,
        CreateTexture specDataFunc
        ) : m_Width(width), m_Height(height)
{
    dataFunc(&m_TexID, m_Width, m_Height, data);
    specDataFunc(&m_SpecTexID, m_Width, m_Height, specularData);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_BindFunc = bind_specDefault;
}

void Texture::setBindFunc(BindTexture func)
{
    m_BindFunc = func;
}

void Texture::bind()
{
    m_BindFunc(m_TexID, m_SpecTexID);
}

GLuint Texture::getTexID() { return m_TexID; }
GLuint Texture::getSpecTexID() { return m_SpecTexID; }
GLuint Texture::getWidth() { return m_Width; }
GLuint Texture::getHeight() { return m_Height; }