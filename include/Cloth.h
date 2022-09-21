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
        vector<unsigned int> structural;
        vector<unsigned int> shear;
        vector<unsigned int> flexion;
    };
    struct vertex {
        glm::vec3 position;
        glm::vec3 normal;
        float mass;
        float velocity;
        spring springs;
    };
    enum Direction {
        TOP, BOTTOM, LEFT, RIGHT
    };
private:
    Shader* mShader;
    Light* mLight;
    float mWidth;
    float mHeight;
    float mStep;
    int mRows;
    int mColumns;
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

    bool isValidIndex(const int&, const Direction&) const;
    int getIndex(const unsigned int&, const Direction&) const;

    void setSprings();
    void setOneDirect_SF(unsigned int, const Direction&);
    void setOneDirect_SH(unsigned int, const Direction&);
    void setStructuralAndFlexionSprings(const unsigned int&);
    void setShearSprings(const unsigned int&);

    // float F_int(const vertex&) const;
    // float F_gr(const vertex&) const;
    // float F_vi(const vertex&) const;

};