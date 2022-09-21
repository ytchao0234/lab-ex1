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
    struct Spring {
        unsigned int index;
        float naturalLength;
    };
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        float mass = 1.0f;
        glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
        vector<Spring> structural;
        vector<Spring> shear;
        vector<Spring> flexion;
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
    vector<Vertex> mVertices;
    vector<unsigned int> mIndices;

    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;
public:
    Cloth();
    ~Cloth();

    void makeVertices();
    void makeIndices();
    void move();
    void bind();
    void render(const glm::mat4&, const glm::mat4&, const glm::vec3&) const;

    bool isValidIndex(const int&, const Direction&) const;
    int getIndex(const unsigned int&, const Direction&) const;

    void setSprings();
    void setStructuralAndFlexionSprings(const unsigned int&);
    void setShearSprings(const unsigned int&);
    void setOneDirect_SF(unsigned int, const Direction&);
    void setOneDirect_SH(unsigned int, const Direction&);

    glm::vec3 F_int(const Vertex&) const;
    glm::vec3 F_int_single(const Vertex&, const Spring&) const;
    glm::vec3 F_gr(const Vertex&) const;
    glm::vec3 F_vi(const Vertex&) const;
};