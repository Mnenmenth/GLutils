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
    // Parent matrix refers to the cumulative matrix of all previous transformations
        // that should also apply to this object in addition to its own global/local transformations
    // ShaderProgram is used to make sure the correct shaders are being used when rendering the object
    virtual void render(ShaderProgram* program, glm::mat4 ParentMatrix) = 0;

};


#endif //GLUTILS_RENDERABLE_H
