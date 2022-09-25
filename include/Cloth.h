#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <Shader.h>
#include <Light.h>

class Cloth
{
public:
    static const float K;
    static const float C;
    static const float G;
    struct Face {
        glm::dvec3 vertices = {0,0,0};
        glm::vec3 normal = {0.0f, 0.0f, 0.0f};;
    };
    struct Spring {
        unsigned int index = 0;
        float naturalLength = 0.0f;
    };
    struct Vertex {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 normal = {0.0f, 0.0f, 0.0f};
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        bool fixed = false;
        float mass = 1.0f;
        glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
        vector<Face> adjacentFaces;
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
    void update();
    void bind();
    void render(const glm::mat4&, const glm::mat4&, const glm::vec3&) const;

    void setFaceList();
    void setNormal();
    glm::vec3 calcuNormal(glm::dvec3);

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