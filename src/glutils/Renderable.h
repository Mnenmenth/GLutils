/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef GLUTILS_RENDERABLE_H
#define GLUTILS_RENDERABLE_H

#include "Transformable.h"

class Renderable : public Transformable {
public:
    Renderable();
    // Parent matrix refers to the cumulative matrix of all previous transformations
        // that should also apply to this object in addition to its own global/local transformations
    virtual void render(glm::mat4 ParentMatrix) = 0;

};


#endif //GLUTILS_RENDERABLE_H
