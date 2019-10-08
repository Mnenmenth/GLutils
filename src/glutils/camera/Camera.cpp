/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Camera.h"

Camera::Camera(glm::vec3 EyePos, glm::vec3 LookingAt, glm::vec3 WorldUp) :
        m_EyePos(EyePos),
        m_LookingAt(LookingAt),
        m_WorldUp(WorldUp),
        m_WorldDirection(glm::normalize(LookingAt-EyePos)),
        m_WorldRight(glm::cross(glm::normalize(LookingAt-EyePos), WorldUp)),
        m_Orientation(glm::angleAxis(0.0f, WorldUp)),
        m_CamTranslation( 0.0f, 0.0f, 0.0f)
{}

/// Shift world
void Camera::moveWorldLeft(float delta) {
    m_CamTranslation -= (delta * m_WorldRight);
}

void Camera::moveWorldRight(float delta) {
    moveWorldLeft(-delta);
}

void Camera::moveWorldUp(float delta) {
    m_CamTranslation += (delta * m_WorldUp);
}

void Camera::moveWorldDown(float delta) {
    moveWorldUp(-delta);
}

void Camera::moveWorldForward(float delta) {
    m_CamTranslation += (delta * m_WorldDirection);
}

void Camera::moveWorldBackward(float delta) {
    moveWorldForward(-delta);
}



/// Rotate World
void Camera::rotateWorldLeft(float theta) {
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldUp));
}

void Camera::rotateWorldRight(float theta) {
    rotateWorldLeft(-theta);
}

void Camera::rotateWorldUp(float theta) {
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldRight));
}

void Camera::rotateWorldDown(float theta) {
    rotateWorldUp(-theta);
}

/// Move Camera
void Camera::moveLeft(float delta) {
    m_CamTranslation += (delta *  m_WorldRight) * m_Orientation;
}

void Camera::moveRight(float delta) {
    moveLeft(-delta);
}

void Camera::moveUp(float delta) {
    m_CamTranslation -= (delta * m_WorldUp) * m_Orientation;
}

void Camera::moveDown(float delta) {
    moveUp(-delta);
}

void Camera::moveForward(float delta) {
    m_CamTranslation -= (delta * m_WorldDirection) * m_Orientation;
}

void Camera::moveBackward(float delta) {
    moveForward(-delta);
}

void Camera::setCamPosition(glm::vec3 pos) {
    m_CamTranslation = -1.0f * pos;
}

/// Rotate Camera
void Camera::rotateLeft(float theta) {
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldUp * m_Orientation));
}

void Camera::rotateRight(float theta) {
    rotateLeft(-theta);
}

void Camera::rotateUp(float theta) {
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldRight * m_Orientation));
}

void Camera::rotateDown(float theta) {
    rotateUp(-theta);
}

void Camera::rollLeft(float theta) {
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldDirection * m_Orientation));
}

void Camera::rollRight(float theta) {
    rollLeft(-theta);
}

void Camera::setOrientation(glm::quat newOrient) {
    m_Orientation = glm::normalize(newOrient);
}

void Camera::resetCamera() {
    m_Orientation = glm::normalize(glm::inverse(m_Orientation));
}

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 view = glm::lookAt(m_EyePos, m_LookingAt, m_WorldUp);
    view = view * glm::mat4_cast(m_Orientation);
    view = glm::translate(view, m_CamTranslation);
    return view;
}