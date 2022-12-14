#include <Light.h>

Light::Light()
{
}

void Light::setAttribute(Shader* shader, glm::vec3 position) const
{
    shader->use();

    shader->setFloatVec("lightPos"  , {position.x, position.y, position.z},    3);
    shader->setFloatVec("lightColor", {mColor.x, mColor.y, mColor.z}      ,    3);

    shader->setFloat("ambientStrength",  mAmbient);
    shader->setFloat("diffuseStrength",  mDiffuse);
    shader->setFloat("specularStrength", mSpecular);
}