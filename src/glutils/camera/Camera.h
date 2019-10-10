/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef GLUTILS_CAMERA_H
#define GLUTILS_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

//TODO: Rename functions
// moveWorld    ->
// rotateWorld  ->
// Rotate       -> set/adjust<Yaw,Pitch,Roll>(float delta)
// fly          ->
// move         ->
// rotate       ->
//              -> add "locked" version of up axis rotation to refer to rotating camera with an unaffected Y position

class Camera {
public:
    // WorldDirection is the base direction vector that the camera is initially facing
    Camera(glm::vec3 EyePos, glm::vec3 LookingAt, glm::vec3 WorldUp);

/// Camera manipulation functions

    /// Moves world by delta relative to starting camera direction vectors.
    /// These movements are not affected by the cameras current orientation
        // Moves world in <direction>.
        // i.e. Move left means world moves left, making camera
        // look like its moving to the right
    void moveWorldLeft(float delta);
    void moveWorldRight(float delta);
    void moveWorldUp(float delta);
    void moveWorldDown(float delta);
    void moveWorldForward(float delta);
    void moveWorldBackward(float delta);

    /// Rotates world by delta
    // Theta is in degrees
    void rotateWorldLeft(float theta);
    void rotateWorldRight(float theta);
    void rotateWorldUp(float theta);
    void rotateWorldDown(float theta);

    /// Move ("fly") camera along its relative axis and orienation
    void flyLeft(float delta);
    void flyRight(float delta);
    void flyUp(float delta);
    void flyDown(float delta);
    void flyForward(float delta);
    void flyBackward(float delta);

    /// Move camera along its relative axis, ignoring delta Y from orientation. Think "walking"
    void moveLeft(float delta);
    void moveRight(float delta);
    // Move up/down are slightly different. They ignore X & Z and move vertical
        // (Really just a alias for moveWorld<Up/Down> for nicer naming. It feels better to
        //  say "Move Up" when, for example, jumping on a key press than "Shift World Down"
        // At least in my opinion it does
    void moveUp(float delta);
    void moveDown(float delta);
    void moveForward(float delta);
    void moveBackward(float delta);

    // Position of camera in world, NOT eye position
    // pos is multiplied by -1 since camera position is actually a translation of the world
    void setCamPosition(glm::vec3 pos);

    /// Rotates camera around its LookingAt point
        //! Direction refers to camera movement. NOT WORLD VIEW CHANGE.
        //! i.e. Rotate up will rotate THE CAMERA up, making the view look down at the world
    // Theta is degrees
    void rotateLeft(float theta);
    void rotateRight(float theta);
    void rotateUp(float theta);
    void rotateDown(float theta);
    void rollLeft(float theta);
    void rollRight(float theta);

    void setOrientation(glm::quat newOrient);
    void resetCamera();

    glm::mat4 getViewMatrix();
    
protected:
    const glm::vec3 m_EyePos;
    const glm::vec3 m_LookingAt;

    const glm::vec3 m_WorldUp;
    const glm::vec3 m_WorldDirection;
    const glm::vec3 m_WorldRight;

    glm::quat       m_Orientation;
    glm::vec3       m_CamTranslation;
};


#endif //GLUTILS_CAMERA_H
