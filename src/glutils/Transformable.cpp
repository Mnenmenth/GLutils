/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Transformable.h"

Transformable::Transformable()
{
    resetTransformations();
}

void Transformable::resetTransformations()
{
    m_globalScale           = glm::vec3(1.0f, 1.0f, 1.0f);
    m_globalOrientation     = glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    m_globalPosition        = glm::vec3(0.0f, 0.0f, 0.0f);

    m_localScale            = glm::vec3(1.0f, 1.0f, 1.0f);
    m_localOrientation      = glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    m_localPosition         = glm::vec3(0.0f, 0.0f, 0.0f);
}

/// Global

// Scale
void Transformable::scaleGlobal(glm::vec3 scale)
{
    // Multiply in the new scale factor
    m_globalScale *= scale;
    // Scale the current position by the new scale factor
    m_globalPosition = m_globalPosition * scale;
}

void Transformable::setGlobalScale(glm::vec3 scale)
{
    // Remove the current scale factors from global position
    m_globalPosition = m_globalPosition / m_globalScale;
    // Set the new scale value
    m_globalScale = scale;
    // Scale the position by the new scale value
    m_globalPosition = m_globalPosition * scale;
}

// Orientation/Rotation

void Transformable::rotateGlobal(glm::quat nOrient)
{
    // Apply rotation and normalize quat
    m_globalOrientation = m_globalOrientation * nOrient;
    m_globalOrientation = glm::normalize(m_globalOrientation);

    // Apply rotation to position
    m_globalPosition    = m_globalPosition * nOrient;
}

void Transformable::rotateGlobal(float angle, glm::vec3 axis)
{
    // Convert euler angles to quat (making sure axis vector is normalized) and then apply rotation
    rotateGlobal(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

void Transformable::setGlobalOrientation(glm::quat orientation)
{
    // Make sure the input orientation is normalized
    orientation = glm::normalize(orientation);
    // Remove the current orientation from the position
    m_globalPosition = m_globalPosition * glm::normalize(glm::conjugate(m_localOrientation) * orientation);
    // Set new orientation
    m_globalOrientation = orientation;
    // Apply new orientation to position
    m_globalPosition = m_globalPosition * orientation;
}

void Transformable::setGlobalOrientation(float angle, glm::vec3 axis)
{
    // Convert euler angles to quat (making sure axis vector is normalized) and then set orientation
    setGlobalOrientation(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

// Translation/Position
void Transformable::translateGlobal(glm::vec3 translation)
{
    // Add in the new translation
    m_globalPosition += translation;
}

void Transformable::setGlobalPosition(glm::vec3 position)
{
    // Set the new position
    m_globalPosition = position;
}



const glm::vec3& Transformable::getGlobalScale()        const { return m_globalScale; }
const glm::quat& Transformable::getGlobalOrientation()  const { return m_globalOrientation; }
const glm::vec3& Transformable::getGlobalPosition()     const { return m_globalPosition; }

/// Local

// Scale
void Transformable::scaleLocal(glm::vec3 scale)
{
    // Multiply in the new scale factor
    m_localScale *= scale;
}

void Transformable::setLocalScale(glm::vec3 scale)
{
    // Set the new scale factor
    m_localScale = scale;
}

// Rotation/Orientation

void Transformable::rotateLocal(glm::quat orientation)
{
    // Apply rotation and normalize quat
    m_localOrientation = m_localOrientation * orientation;
    m_localOrientation = glm::normalize(m_localOrientation);
}

void Transformable::rotateLocal(float angle, glm::vec3 axis)
{
    // Convert euler angles to quat (making sure axis vector is normalized) and then apply rotation
    rotateLocal(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

void Transformable::setLocalOrientation(glm::quat orientation)
{
    // Set new orientation, making sure the new quat is normalized
    m_localOrientation = glm::normalize(orientation);
}

void Transformable::setLocalOrientation(float angle, glm::vec3 axis)
{
    // Convert euler angles to quat (making sure axis vector is normalized) and then set new orientation
    setLocalOrientation(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

// Translation/Position

void Transformable::translateLocal(glm::vec3 translation)
{
    // Apply new translation
    m_localPosition += translation;
}

void Transformable::setLocalPosition(glm::vec3 position)
{
    // Set the new position
    m_localPosition = position;
}

const glm::vec3& Transformable::getLocalScale()         const { return m_localScale; }
const glm::quat& Transformable::getLocalOrientation()   const { return m_localOrientation; }
const glm::vec3& Transformable::getLocalPosition()      const { return m_localPosition; }

/// Combined transformations
glm::quat Transformable::getScale()         const { return m_globalScale * m_localScale; }
glm::quat Transformable::getOrientation()   const { return m_globalOrientation * m_localOrientation; }
glm::quat Transformable::getPosition()      const { return m_globalPosition + m_localPosition; }

glm::mat4 Transformable::getGlobalMatrix()
{
    glm::mat4 Global(1.0f);
    Global = glm::translate(Global, m_globalPosition);
    Global = Global * glm::mat4_cast(m_globalOrientation);
    Global = glm::scale(Global, m_globalScale);

    return Global;
}

glm::mat4 Transformable::getLocalMatrix()
{
    glm::mat4 Local(1.0f);
    Local = glm::translate(Local, m_localPosition);
    Local = Local * glm::mat4_cast(m_localOrientation);
    Local = glm::scale(Local, m_localScale);

    return Local;
}

glm::mat4 Transformable::getTransformationMatrix()
{
    return getGlobalMatrix() * getLocalMatrix();
}