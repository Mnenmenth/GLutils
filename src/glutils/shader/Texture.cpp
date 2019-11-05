/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include <iostream>
#include "Texture.h"

void Texture::createTexture_default(GLuint* texID, unsigned char data[])
{
    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_2D, *texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

}

void Texture::bind_default(GLuint texID, GLuint specTexID)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specTexID);
}

Texture::Texture(unsigned char data[], void (*createTexture)(GLuint*, unsigned char[])) : Texture(data, data, createTexture, createTexture)
{
}

Texture::Texture(
        unsigned char data[],
        unsigned char specularData[],
        void (*createTexture)(GLuint*, unsigned char[]),
        void (*createSpecTexture)(GLuint*, unsigned char[])
        )
{
    createTexture(&m_TexID, data);
    createSpecTexture(&m_SpecTexID, specularData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind()
{
    m_BindFunc(m_TexID, m_SpecTexID);
}

void Texture::setBindFunc(void (*bindFunc)(GLuint, GLuint))
{
    m_BindFunc = bindFunc;
}