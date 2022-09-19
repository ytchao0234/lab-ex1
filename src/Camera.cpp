#include <Camera.h>

Camera::Camera(glm::vec3 position, float yaw, float pitch): mZoomValue(10)
{
    mPosition = position;
    mYawValue = yaw;
    mPitchValue = pitch;
    
    glm::vec3 dTemp;
    dTemp.x = cos(glm::radians(mYawValue)) * cos(glm::radians(mPitchValue));
    dTemp.y = sin(glm::radians(mPitchValue));
    dTemp.z = sin(glm::radians(mYawValue)) * cos(glm::radians(mPitchValue));
    mDirection = glm::normalize(dTemp);

    mTarget = mPosition + mDirection;
}

void Camera::zoom(float value)
{
    mZoomValue -= (float)value * 0.5;

    if (mZoomValue < 0.1f)
        mZoomValue = 0.1f;
    else if (mZoomValue > 50.0f)
        mZoomValue = 50.0f;
}

void Camera::move(float offsetX, float offsetY)
{
    mYawValue   += offsetX;
    mPitchValue += offsetY;
    
    if(mPitchValue > 89.0f)
        mPitchValue =  89.0f;
    if(mPitchValue < -89.0f)
        mPitchValue = -89.0f;
    
    glm::vec3 dTemp;
    dTemp.x = cos(glm::radians(mYawValue)) * cos(glm::radians(mPitchValue));
    dTemp.y = sin(glm::radians(mPitchValue));
    dTemp.z = sin(glm::radians(mYawValue)) * cos(glm::radians(mPitchValue));
    mDirection = glm::normalize(dTemp);

    mPosition = mTarget - mDirection;
}

glm::mat4 Camera::getProjection(int width, int height) const
{
    glm::mat4 projection;

    // projection = glm::perspective(45.0f, (float)width/height, 0.1f, 400.0f);

    if( width > height )
    {
        projection = glm::ortho(-10.0f*mZoomValue, 10.0f*mZoomValue,
                                -10.0f*mZoomValue * ((float)height/width), 10.0f*mZoomValue * ((float)height/width),
                                -400.0f, 400.0f);
    }
    else
        projection = glm::ortho(-10.0f*mZoomValue * ((float)width/height), 10.0f*mZoomValue * ((float)width/height),
                                -10.0f*mZoomValue, 10.0f*mZoomValue,
                                -400.0f, 400.0f);

    return projection;
}

glm::mat4 Camera::getView() const
{
    glm::vec3 right = glm::normalize(glm::cross(mDirection, mViewUp));
    glm::vec3 up = glm::cross(right, mDirection);
    
    glm::mat4 view = glm::lookAt(mPosition, mTarget, up);
    return view;
}