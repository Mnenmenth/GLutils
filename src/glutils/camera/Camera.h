/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef GLUTILS_CAMERA_H
#define GLUTILS_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
public:
    Camera(glm::vec3 EyePos, glm::vec3 LookingAt, glm::vec3 WorldUp);
    Camera(glm::vec3 EyePos, glm::vec3 LookingAt, glm::vec3 WorldUp, glm::vec3 WorldForward, glm::vec3 WorldRight);

/// Camera manipulation functions
//!! 'Lat' is an abbreviation for 'Lateral', for left/right movement
//!! 'Vert' is an abbreviation for 'Vertical', for up/down movement
//!! 'Long' is an abbreviation for 'Longitudinal', for forward/back movement
//TODO:
// 1)   Add ability to reset roll/pitch/yaw individually

    /// Moves world by delta relative to direction vectors derived from class parameters
        //! These movements are not affected by the cameras current orientation

    void moveWorld(glm::vec3 delta);
    void moveWorldLat(float delta);
    void moveWorldVert(float delta);
    void moveWorldLong(float delta);

    /// Rotates world by delta around WorldUp, WorldRight, and WorldForward direction vectors
        //! Theta is in degrees

    //void rotateWorld(glm::quat orientation);
    void rotateWorldYaw(float theta);
    void rotateWorldPitch(float theta);
    //TODO: Include rotateWorldRoll and have it rotate around (EyePos - CamPos)?
    // World direction vectors would need to be updated

    /// Move camera along its relatively oriented axis

    void move(glm::vec3 delta);
    void moveLat(float delta);
    void moveVert(float delta);
    void moveLong(float delta);

    /// Move camera along its relative axis, ignoring components from perpendicular axis
    /// to effectively "lock" its values along those axis
        //! Lat & Long ignore Y, Vert ignores X & Z
        // Think of it as how in an FPS the camera/character don't fly off into the sky
        // while walking around, regardless of where the camera is facing. They're "locked"
        // onto the ground

    void moveLocked(glm::vec3 delta);
    void moveLatLocked(float delta);
    void moveVertLocked(float delta);
    void moveLongLocked(float delta);

    /// Position of camera in world, NOT eye position
        // pos is inverted since camera position is actually a translation of the world,
        // so the camera's translation is the inverse of where it appears to be

    void setCamPosition(glm::vec3 pos);

    /// Rotates camera around its LookingAt point
        //! Theta is in degrees
        // Be aware that with a first person EyePos & LookingAt configuration
        // "looking left" is a positive rotation and "looking right" is a negative
        // rotation since rotating the camera left makes it "look" to the right

    void rotate(glm::quat orientation);
    void rotateYaw(float theta);
    void rotatePitch(float theta);
    void rotateRoll(float theta);

    void setOrientation(glm::quat newOrient);
    void resetOrientation();

    /// Moves eye position closer to and farther away from "LookingAt"
    //! Min defaults to .01f and Max defaults to 100.0f
    void zoom(float delta);
    void setZoom(float newZoom);
    void setMinZoom(float zoomMin);
    void setMaxZoom(float zoomMax);
    void resetZoom();

    /// Getters
    glm::vec3 getEyePos();
    glm::vec3 getLookingAt();
    glm::vec3 getWorldUp();
    glm::vec3 getWorldForward();
    glm::vec3 getWorldRight();
    glm::vec3 getCamTranslation();
    glm::quat getOrientation();
    float getZoom();
    glm::mat4 getViewMatrix();

protected:
    glm::vec3 m_EyePos;
    const glm::vec3 m_LookingAt;

    const glm::vec3 m_WorldUp;
    const glm::vec3 m_WorldForward;
    const glm::vec3 m_WorldRight;

    glm::quat       m_Orientation;
    glm::vec3       m_CamTranslation;

    float m_Zoom;
    float m_ZoomMin;
    float m_ZoomMax;
};


#endif //GLUTILS_CAMERA_H
