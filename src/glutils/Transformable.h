/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef GLUTILS_TRANSFORMABLE_H
#define GLUTILS_TRANSFORMABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <utility>

class Transformable {
public:
    Transformable();

/// Global transformations
    void scaleGlobal(glm::vec3 scale);
    void setGlobalScale(glm::vec3 scale);

    void rotateGlobal(glm::quat orientation);
    void rotateGlobal(float angle, glm::vec3 axis);
    void setGlobalOrientation(glm::quat orientation);
    void setGlobalOrientation(float angle, glm::vec3 axis);

    void translateGlobal(glm::vec3 translation);
    void setGlobalPosition(glm::vec3 position);

    // Relative to world origin. Object may not actually be at this
        // scale/orientation/position depending on local transformations
    const glm::vec3& getGlobalScale() const;
    const glm::quat& getGlobalOrientation() const;
    const glm::vec3& getGlobalPosition() const;

/// Local transformations
    void scaleLocal(glm::vec3 scale);
    void setLocalScale(glm::vec3 scale);

    void rotateLocal(glm::quat orientation);
    void rotateLocal(float angle, glm::vec3 axis);
    void setLocalOrientation(glm::quat orientation);
    void setLocalOrientation(float angle, glm::vec3 axis);

    void translateLocal(glm::vec3 translation);
    void setLocalPosition(glm::vec3 position);

    // Relative to objects local origin. Object may not be actually be at this
        // scale/orientation/position depending on local transformations
    const glm::vec3& getLocalScale() const;
    const glm::quat& getLocalOrientation() const;
    const glm::vec3& getLocalPosition() const;

/// Combined global and local transformations
    glm::quat getScale() const;
    glm::quat getOrientation() const;
    glm::quat getPosition() const;

    // Global Transformation Matrix
    glm::mat4 getGlobalMatrix();
    // Local Transformation Matrix
    glm::mat4 getLocalMatrix();
    // Combined Global and Local Transformations Matrix
    glm::mat4 getTransformationMatrix();
    void resetTransformations();

private:
/// Global transformations
    glm::vec3 m_globalScale;
    glm::quat m_globalOrientation;
    glm::vec3 m_globalPosition;

/// Local transformations
    glm::vec3 m_localScale;
    glm::quat m_localOrientation;
    glm::vec3 m_localPosition;
};


#endif //GLUTILS_TRANSFORMABLE_H
