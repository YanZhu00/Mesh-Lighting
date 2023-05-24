#pragma once
#include "Hazel.h"
#include "Scene/Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Vertex
{
    float x, y, z;
};

struct VertexNormal
{
    float x, y, z;
};

struct Face
{
    unsigned x, y, z;
};

struct FaceNormal
{
    unsigned x, y, z;
};


class Object
{
public:
    Object();
    void ReadFile(const char* filename);
    void Draw(std::shared_ptr<Hazel::Shader> shader, CameraMatrices matrices, bool left);
    void ResetScale();

    glm::vec3 m_Color;
    float m_Scale;

private:
    std::vector<float> m_Vertices;
    std::vector<float> m_Normals;
    std::vector<unsigned int> m_Faces;
    std::vector<unsigned int> m_FaceNormals;
    std::vector<unsigned int> m_F;

    std::shared_ptr<Hazel::VertexArray> m_VertexArray;
    std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
    std::shared_ptr<Hazel::VertexBufferLayout> m_Layout;
    std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;

    bool m_read = false;
};

