#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <Light.h>

class Cloth
{
public:
    static const float K;
    static const float G;
    struct spring {
        vector<glm::dvec2> structural;
        vector<glm::dvec2> shear;
        vector<glm::dvec2> flexion;
    };
    struct vertex {
        glm::dvec2 index;
        glm::vec3 position;
        glm::vec3 normal;
        float mass;
        float velocity;
        spring springs;
    };
private:
    Shader* mShader;
    Light* mLight;
    float mWidth;
    float mHeight;
    float mStep;
    vector<vertex> mVertices;
    vector<unsigned int> mIndices;

    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;
public:
    Cloth();
    ~Cloth();

    void makeVertices();
    void makeIndices();
    void bind();
    void render(const glm::mat4&, const glm::mat4&, const glm::vec3&) const;
    void setSprings();

    // float F_int(const vertex&) const;
    // float F_gr(const vertex&) const;
    // float F_vi(const vertex&) const;

    vector<glm::dvec2> getStructuralSprings(const glm::dvec2&) const;
    vector<glm::dvec2> getShearSprings(const glm::dvec2&) const;
    vector<glm::dvec2> getFlexionSprings(const glm::dvec2&) const;
};