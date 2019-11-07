/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef GLUTILS_RENDERABLE_H
#define GLUTILS_RENDERABLE_H

#include "Transformable.h"
#include "shader/ShaderProgram.h"

class Renderable : public Transformable
{
public:
    explicit Renderable(glm::vec3 baseSize) : m_BaseSize(baseSize) {}
    // Parent matrix refers to the cumulative matrix of all previous transformations
        // that should also apply to this object in addition to its own global/local transformations
    // ShaderProgram is used to make sure the correct shaders are being used when rendering the object
    virtual void render() = 0;
    virtual glm::vec3 getSize() { return m_BaseSize * getScale(); };
    virtual void setBaseSize(glm::vec3 size) { m_BaseSize = size; }
    virtual glm::vec3 getBaseSize() { return m_BaseSize; }

protected:
    glm::vec3 m_BaseSize;
};


#endif //GLUTILS_RENDERABLE_H
