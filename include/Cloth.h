#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>

class Cloth
{
public:
    struct vertex {
        int index = 0;
        int i = 0;
        int j = 0;
        glm::vec3 normal = {0.0f, 0.0f, 1.0f};
    };
private:
    Shader* mShader;
    vector<float> mVertices;
    vector<vertex> mAdjacency;

    unsigned int mVAO[1];
    unsigned int mVBO[1];
public:
    Cloth();
    ~Cloth();

    void makeVertices();
    void bindVertices();
    void render(const glm::mat4&, const glm::mat4&) const;
};