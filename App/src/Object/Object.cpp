#include "Object.h"

Object::Object()
{
    m_Color = glm::vec3(1., 1., 1.);
}


void Object::ReadFile(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }

    float xMax = 0;
    float yMax = 0;
    float zMax = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v') {
            if (line[1] == ' ') {
                float x, y, z;
                sscanf(line, "v %f %f %f", &x, &y, &z);
                if (abs(x) > xMax) xMax = abs(x);
                if (abs(y) > yMax) yMax = abs(y);
                if (abs(z) > zMax) zMax = abs(z);
                m_Vertices.push_back(x);
                m_Vertices.push_back(y);
                m_Vertices.push_back(z);
            }
            else if (line[1] == 'n') {
                float x, y, z;
                sscanf(line, "vn %f %f %f", &x, &y, &z);
                m_Normals.push_back(x);
                m_Normals.push_back(y);
                m_Normals.push_back(z);
            }
        }
        else if (line[0] == 'f') {
            unsigned int f1, f2, f3, fn1, fn2, fn3;
            sscanf(line, "f %d//%d %d//%d %d//%d",
                &f1, &fn1,
                &f2, &fn2,
                &f3, &fn3);
            m_Faces.push_back(f1 - 1);
            m_Faces.push_back(f2 - 1);
            m_Faces.push_back(f3 - 1);
            m_FaceNormals.push_back(fn1 - 1);
            m_FaceNormals.push_back(fn2 - 1);
            m_FaceNormals.push_back(fn3 - 1);
        }
    }

    fclose(file);


    const int n = m_Faces.size();
    std::vector<float> vertexbuffer(6 * n);
    for (int i = 0; i < n; i++)
    {
        vertexbuffer[i * 6 + 0] = m_Vertices[m_Faces[i] * 3 + 0];
        vertexbuffer[i * 6 + 1] = m_Vertices[m_Faces[i] * 3 + 1];
        vertexbuffer[i * 6 + 2] = m_Vertices[m_Faces[i] * 3 + 2];
        vertexbuffer[i * 6 + 3] = m_Normals[m_FaceNormals[i] * 3 + 0];
        vertexbuffer[i * 6 + 4] = m_Normals[m_FaceNormals[i] * 3 + 1];
        vertexbuffer[i * 6 + 5] = m_Normals[m_FaceNormals[i] * 3 + 2];
    }
    std::vector<unsigned int> indexbuffer(n * 6);
    for (int i = 0; i < 6 * n; i++)
    {
        indexbuffer[i] = i;
    }


    m_VertexArray = std::make_shared<Hazel::VertexArray>();
    m_VertexBuffer = std::make_shared<Hazel::VertexBuffer>(&vertexbuffer[0], vertexbuffer.size() * sizeof(float));
    m_IndexBuffer = std::make_shared<Hazel::IndexBuffer>(&indexbuffer[0], indexbuffer.size());
    m_Layout = std::make_shared<Hazel::VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(3);
    m_VertexArray->AddBuffer(*m_VertexBuffer, *m_Layout);

    float max = std::max({ xMax, yMax, zMax });
    m_Scale = 3 / (max + 0.0000001f);
    m_read = true;
}

void Object::Draw(std::shared_ptr<Hazel::Shader> shader, CameraMatrices matrices, bool left)
{
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(m_Scale, m_Scale, m_Scale));
    ModelMatrix = left ? matrices.CameraModelMatrix * ModelMatrix : ModelMatrix * matrices.CameraModelMatrix;

    shader->Bind();
    shader->SetUniformMatrix4fv("ProjectionMatrix", &matrices.ProjectionMatrix[0][0]);
    shader->SetUniformMatrix4fv("ViewMatrix", &matrices.ViewMatrix[0][0]);
    shader->SetUniformMatrix4fv("ModelMatrix", &ModelMatrix[0][0]);
    shader->SetUniform3f("ObjectColor", m_Color[0], m_Color[1], m_Color[2]);

    if (m_read) Hazel::Renderer::Draw(*m_VertexArray, *m_IndexBuffer, *shader);
}


void Object::ResetScale()
{
    m_Scale = 1.f;
}