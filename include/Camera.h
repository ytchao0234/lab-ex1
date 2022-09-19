#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    glm::vec3 mPosition;
    glm::vec3 mTarget;
    glm::vec3 mDirection;
    glm::vec3 mViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float mYawValue;
    float mPitchValue;
    float mZoomValue;

public:
    Camera(glm::vec3, float, float);

    void zoom(float);
    void move(float, float);
    
    glm::mat4 getProjection(int, int) const;
    glm::mat4 getView() const;

    glm::vec3 getPosition() const { return mPosition; }
    glm::vec3 getTarget() const { return mTarget; }
    glm::vec3 getDirection() const { return mDirection; }
    float getYaw() const { return mYawValue; }
    float getPitch() const { return mPitchValue; }
    float getZoom() const { return mZoomValue; }
};