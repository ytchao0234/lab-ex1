#include <Cloth.h>

const float Cloth::K = 2.5f;
const float Cloth::C = 0.5f;
const float Cloth::G = 9.8f;

Cloth::Cloth()
{
    mWidth = 50.0f;
    mHeight = 50.0f;
    mStep = 10.0f;
    mRows = mHeight / mStep + 1;
    mColumns = mWidth / mStep + 1;

    mShader = new Shader("src/Shaders/cloth.vert", "src/Shaders/cloth.frag");
    mLight = new Light();
}

void Cloth::makeVertices()
{
    Vertex v;

    for(float i = -mWidth / 2; i <= mWidth / 2; i += mStep)
    {
        for(float j = -mHeight / 2; j <= mHeight / 2; j += mStep)
        {
            v.position = glm::vec3(i, j, 0.0f);
            mVertices.push_back(v);
        }
    }

    mVertices[mRows - 1].fixed = true;
    mVertices[mRows * mColumns - 1].fixed = true;

    this->makeIndices();
    this->setFaceList();
    this->setNormal();
    this->setSprings();
    this->bind();
}

void Cloth::makeIndices()
{
    unsigned int right, topRight, top;

    for(unsigned int i = 0; i < mVertices.size(); i += 1)
    {
        if(i % mRows + 1 >= mRows || i / mRows + 1 >= mColumns)
            continue;

        right = this->getIndex(i, Direction::RIGHT);
        topRight = this->getIndex(right, Direction::TOP);
        top = this->getIndex(i, Direction::TOP);

        mIndices.insert(mIndices.end(), {i, right, topRight});
        mIndices.insert(mIndices.end(), {topRight, top, i});
    }
}

void Cloth::setFaceList()
{
    int i = 0, j = 0, v = 0, f = 0;
    Face face;

    for(i = 0; i < (int)mIndices.size(); i += 1)
    {
        v = mIndices[i];
        f = j - j % 3;
        face.vertices = {mIndices[f], mIndices[f + 1], mIndices[f + 2]};
        mVertices[v].adjacentFaces.push_back(face);
    }
}

void Cloth::setNormal()
{
    glm::vec3 sumOfNormal = {0.0f, 0.0f, 0.0f};

    for(auto& vertex: mVertices)
    {
        for(auto& face: vertex.adjacentFaces)
        {
            face.normal = calcuNormal(face.vertices);
            sumOfNormal += face.normal;
        }
        vertex.normal = glm::normalize(sumOfNormal);
    }
}

glm::vec3 Cloth::calcuNormal(glm::dvec3 vertices)
{
    glm::vec3 p0 = mVertices[vertices.x].position;
    glm::vec3 p1 = mVertices[vertices.y].position;
    glm::vec3 p2 = mVertices[vertices.z].position;

    glm::vec3 v0 = p1 - p0;
    glm::vec3 v1 = p2 - p1;

    glm::vec3 cross = glm::cross(v0, v1);
    glm::vec3 normal = glm::normalize(cross);

    return normal;
}

void Cloth::update()
{
    glm::vec3 force = {0.0f, 0.0f, 0.0f};
    glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};
    float delta_t = 0.02f;

    for(int i = 0; i < (int)mVertices.size(); i += 1)
    {
        if(mVertices[i].fixed)
            continue;

        acceleration = force / mVertices[i].mass;
        mVertices[i].velocity += delta_t * acceleration;
        mVertices[i].position += delta_t * mVertices[i].velocity;
        this->setNormal();
    }

    this->bind();
}

void Cloth::bind()
{
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mIndices.size()), GL_UNSIGNED_INT, 0);
}

bool Cloth::isValidIndex(const int& index, const Direction& direction) const
{
    if(index < 0 || index >= mRows * mColumns)
        return false;
    if(direction == Direction::TOP && index % mRows == 0)
        return false;
    if(direction == Direction::BOTTOM && index % mRows == mRows - 1)
        return false;

    return true;
}

int Cloth::getIndex(const unsigned int& index, const Direction& direction) const
{
    switch(direction)
    {
        case TOP:
            return (int)index + 1;
        case BOTTOM:
            return (int)index - 1;
        case LEFT:
            return (int)index - mRows;
        case RIGHT:
            return (int)index + mRows;
    }
}

void Cloth::setSprings()
{
    for(int i = 0; i < (int)mVertices.size(); i += 1)
    {
        setStructuralAndFlexionSprings(i);
        setShearSprings(i);
    }
}

void Cloth::setStructuralAndFlexionSprings(const unsigned int& index)
{
    this->setOneDirect_SF(index, Direction::TOP);
    this->setOneDirect_SF(index, Direction::BOTTOM);
    this->setOneDirect_SF(index, Direction::LEFT);
    this->setOneDirect_SF(index, Direction::RIGHT);
}

void Cloth::setShearSprings(const unsigned int& index)
{
    setOneDirect_SH(index, Direction::TOP);
    setOneDirect_SH(index, Direction::BOTTOM);
}

void Cloth::setOneDirect_SF(unsigned int index, const Direction& direct)
{
    glm::vec3 Pij = mVertices[index].position;
    glm::vec3 Pkl;

    Spring target;
    target.index = this->getIndex(index, direct);

    if(this->isValidIndex(target.index, direct))
    {
        Pkl = mVertices[target.index].position;
        target.naturalLength = glm::distance(Pij, Pkl);
        mVertices[index].structural.push_back(target);

        target.index = this->getIndex(target.index, direct);

        if(this->isValidIndex(target.index, direct))
        {
            Pkl = mVertices[target.index].position;
            target.naturalLength = glm::distance(Pij, Pkl);
            mVertices[index].flexion.push_back(target);
        }
    }
}

void Cloth::setOneDirect_SH(unsigned int index, const Direction& direct)
{
    glm::vec3 Pij = mVertices[index].position;
    glm::vec3 Pkl;
    Spring target, targetLeft, targetRight;
    target.index = this->getIndex(index, direct);

    if(isValidIndex(target.index, direct))
    {
        targetLeft.index  = getIndex(target.index, Direction::LEFT);
        targetRight.index = getIndex(target.index, Direction::RIGHT);

        if(isValidIndex(targetLeft.index, Direction::LEFT))
        {
            Pkl = mVertices[targetLeft.index].position;
            targetLeft.naturalLength = glm::distance(Pij, Pkl);
            mVertices[index].shear.push_back(targetLeft);
        }
        if(isValidIndex(targetRight.index, Direction::RIGHT))
        {
            Pkl = mVertices[targetRight.index].position;
            targetRight.naturalLength = glm::distance(Pij, Pkl);
            mVertices[index].shear.push_back(targetRight);
        }
    }
}

glm::vec3 Cloth::F_int(const Vertex& Pij) const
{
    int i = 0;
    glm::vec3 result = {0.0f, 0.0f, 0.0f};

    for(i = 0; i < (int)Pij.structural.size(); i += 1)
    {
        result += F_int_single(Pij, Pij.structural[i]);
    }
    for(i = 0; i < (int)Pij.shear.size(); i += 1)
    {
        result += F_int_single(Pij, Pij.shear[i]);
    }
    for(i = 0; i < (int)Pij.flexion.size(); i += 1)
    {
        result += F_int_single(Pij, Pij.flexion[i]);
    }  

    return result;
}

glm::vec3 Cloth::F_int_single(const Vertex& Pij, const Spring& spring) const
{
    Vertex Pkl = mVertices[spring.index];
    glm::vec3 l_ijkl = Pkl.position - Pij.position;
    glm::vec3 delta_x = l_ijkl - spring.naturalLength * glm::normalize(l_ijkl);
    glm::vec3 result = Cloth::K * delta_x;

    return result;
}

glm::vec3 Cloth::F_gr(const Vertex& Pij) const
{
    glm::vec3 result = glm::vec3(0.0f, - Pij.mass * Cloth::G, 0.0f);

    return result;
}

glm::vec3 Cloth::F_vi(const Vertex& Pij) const
{
    glm::vec3 result = -Cloth::C * Pij.velocity;

    return result;
}
