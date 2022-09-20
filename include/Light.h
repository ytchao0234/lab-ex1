#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>

class Light
{
private:
    glm::vec3 mPosition = {0.0f, 1.0f, 0.0f};
    glm::vec3 mColor = {1.0f, 1.0f, 1.0f};
    float mAmbient = 0.2f;
    float mDiffuse = 0.3f;
    float mSpecular = 0.5f;

public:
    Light();
    void bind(const unsigned int&);
    void setAttribute(Shader*, glm::vec3) const;
};