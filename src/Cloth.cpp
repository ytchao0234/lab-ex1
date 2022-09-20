#include <Cloth.h>

const float Cloth::K = 2.5f;
const float Cloth::G = 9.8f;

Cloth::Cloth()
{
    mWidth = 50.0f;
    mHeight = 50.0f;
    mStep = 5.0f;
    mShader = new Shader("src/Shaders/cloth.vert", "src/Shaders/cloth.frag");
    mLight = new Light();
}

void Cloth::makeVertices()
{
    vertex v;
    int ii = 0, jj = 0;

    for(float i = -mWidth / 2; i <= mWidth / 2; i += mStep, ii += 1)
    {
        for(float j = -mHeight / 2; j <= mHeight / 2; j += mStep, jj += 1)
        {
            v.index = {ii, jj};
            v.position = glm::vec3(i, j, 0.0f);
            mVertices.push_back(v);
        }
    }

    this->makeIndices();
    this->bind();
}

void Cloth::makeIndices()
{
    int rows = mHeight / mStep + 1;
    int columns = mWidth / mStep + 1;

    for(unsigned int i = 0; i < mVertices.size(); i += 1)
    {
        if(i % rows + 1 >= rows || i / rows + 1 >= columns)
            continue;
        mIndices.push_back(i);
        mIndices.push_back(i + rows);
        mIndices.push_back(i + rows + 1);
        mIndices.push_back(i + 1);
    }
}

void Cloth::bind()
{
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
    glBindVertexArray(0);
}

void Cloth::render(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& position) const
{
    mShader->use();

    mShader->setMatrix4("projection", (float*)glm::value_ptr(projection));
    mShader->setMatrix4("view", (float*)glm::value_ptr(view));

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    mShader->setMatrix4("model", (float*)glm::value_ptr(model));

    mLight->setAttribute(mShader, position);

    glDisable(GL_CULL_FACE);

    glBindVertexArray(mVAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_QUADS, static_cast<unsigned int>(mIndices.size()), GL_UNSIGNED_INT, 0);
}

bool checkValid(glm::dvec2 index) 
{
    return index.x >= 0 && index.y >= 0;
}

vector<glm::dvec2> Cloth::getStructuralSprings(const glm::dvec2& vertex) const
{
    vector<glm::dvec2> structuralSprings;

    if(checkValid({vertex.x + 1, vertex.y}))
        structuralSprings.push_back({vertex.x + 1, vertex.y});
    if(checkValid({vertex.x - 1, vertex.y}))
        structuralSprings.push_back({vertex.x - 1, vertex.y});
    if(checkValid({vertex.x, vertex.y + 1}))
        structuralSprings.push_back({vertex.x, vertex.y + 1});
    if(checkValid({vertex.x, vertex.y - 1}))
        structuralSprings.push_back({vertex.x, vertex.y - 1});

    return structuralSprings;
}

vector<glm::dvec2> Cloth::getShearSprings(const glm::dvec2& vertex) const
{
    vector<glm::dvec2> shearSprings;

    if(checkValid({vertex.x + 1, vertex.y + 1}))
        shearSprings.push_back({vertex.x + 1, vertex.y + 1});
    if(checkValid({vertex.x - 1, vertex.y - 1}))
        shearSprings.push_back({vertex.x - 1, vertex.y - 1});

    return shearSprings;
}

vector<glm::dvec2> Cloth::getFlexionSprings(const glm::dvec2& vertex) const
{
    vector<glm::dvec2> flexionSprings;

    if(checkValid({vertex.x + 2, vertex.y}))
        flexionSprings.push_back({vertex.x + 2, vertex.y});
    if(checkValid({vertex.x - 2, vertex.y}))
        flexionSprings.push_back({vertex.x - 2, vertex.y});
    if(checkValid({vertex.x, vertex.y + 2}))
        flexionSprings.push_back({vertex.x, vertex.y + 2});
    if(checkValid({vertex.x, vertex.y - 2}))
        flexionSprings.push_back({vertex.x, vertex.y - 2});

    return flexionSprings;
}

void Cloth::setSprings()
{
    for(int i = 0; i < (int)mVertices.size(); i += 1)
    {
        mVertices[i].springs.structural = getStructuralSprings(mVertices[i].index);
        mVertices[i].springs.shear = getShearSprings(mVertices[i].index);
        mVertices[i].springs.flexion = getFlexionSprings(mVertices[i].index);
    }
}

// float Cloth::F_int(const vertex& Pij) const
// {
//     int i = 0;
//     glm::vec3 Pkl = {0.0f, 0.0f, 0.0f};

//     for(i = 0; i < (int)Pij.structuralSprings.size(); i += 1)
//     {
//     }

//     for(i = 0; i < (int)Pij.shearSprings.size(); i += 1)
//     {
        
//     }

//     for(i = 0; i < (int)Pij.flexionSprings.size(); i += 1)
//     {
        
//     }

//     return 0;
// }

// float Cloth::F_gr(const vertex& Pij) const
// {
//     return 0;
// }

// float Cloth::F_vi(const vertex& Pij) const
// {
//     return 0;

// }