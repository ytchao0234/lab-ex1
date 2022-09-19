#include <Cloth.h>

Cloth::Cloth()
{
    mShader = new Shader("src/Shaders/cloth.vert", "src/Shaders/cloth.frag");
}

void Cloth::makeVertices()
{
    float width = 50.0f, height = 50.0f;
    float step = 10.0f;
    int index = 0, ii = 0, jj = 0;

    for(float i = -width / 2; i < width / 2; i += step, ii += 1)
    {
        for(float j = -height / 2; j < height / 2; j += step, jj += 1)
        {
            vector<float> square =
            {
                i, j, 0.0f,
                i, j + step, 0.0f,
                i + step, j + step, 0.0f,
                i + step, j, 0.0f,
            };

            vector<vertex> vert =
            {
                {index++, ii, jj, },
                {index++, ii, jj + 1, },
                {index++, ii + 1, jj + 1, },
                {index++, ii + 1, jj, },
            };

            mVertices.insert(mVertices.end(), square.begin(), square.end());
            mAdjacency.insert(mAdjacency.end(), vert.begin(), vert.end());
        }
    }

    this->setSprings();
    this->bindVertices();
}

void Cloth::bindVertices()
{
    glGenVertexArrays(1, mVAO);
    glBindVertexArray(mVAO[0]);

    glGenBuffers(1, mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


void Cloth::render(const glm::mat4& projection, const glm::mat4& view) const
{
    mShader->use();

    mShader->setMatrix4("projection", (float*)glm::value_ptr(projection));
    mShader->setMatrix4("view", (float*)glm::value_ptr(view));

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    mShader->setMatrix4("model", (float*)glm::value_ptr(model));

    glDisable(GL_CULL_FACE);

    glBindVertexArray(mVAO[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_QUADS, 0, mVertices.size() / 3);
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
    glm::dvec2 vertex = {0, 0};

    for(int i = 0; i < (int)mAdjacency.size(); i += 1)
    {
        vertex = {mAdjacency[i].i, mAdjacency[i].j};

        mAdjacency[i].structuralSprings = getStructuralSprings(vertex);
        mAdjacency[i].shearSprings = getShearSprings(vertex);
        mAdjacency[i].flexionSprings = getFlexionSprings(vertex);
    }
}