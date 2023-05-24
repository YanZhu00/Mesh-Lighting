#include "Object.h"

std::vector<std::string> splitString(const std::string& str, const std::string& token) {
    std::vector<std::string> substrings;
    std::size_t startPos = 0;
    std::size_t spacePos = str.find(token);

    while (spacePos != std::string::npos) {
        std::string substring = str.substr(startPos, spacePos - startPos);

        substrings.push_back(substring);

        startPos = spacePos + token.length();
        spacePos = str.find(token, startPos);
    }

    std::string lastSubstring = str.substr(startPos);
    if (lastSubstring[0] != '\n')
        substrings.push_back(lastSubstring);

    return substrings;
}

Object::Object()
{
    m_Color = glm::vec3(1., 1., 1.);
}


void Object::ReadFile(std::string filename)
{
    FILE* file = fopen(filename.c_str(), "r");
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }

    std::vector<float> v;
    std::vector<float> vn;
    std::vector<int> vidx;
    std::vector<int> vnidx;
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v') {
            if (line[1] == ' ') {
                float x, y, z;
                sscanf(line, "v %f %f %f", &x, &y, &z);
                v.push_back(x);
                v.push_back(y);
                v.push_back(z);
            }
            else if (line[1] == 'n') {
                float x, y, z;
                sscanf(line, "vn %f %f %f", &x, &y, &z);
                vn.push_back(x);
                vn.push_back(y);
                vn.push_back(z);
            }
        }
        else if (line[0] == 'f') {
            std::string faceString(line);
            faceString = faceString.substr(2);
            std::vector<std::string> subStrings = splitString(faceString, " ");
            std::vector<int> vidxTemp;
            std::vector<int> vnidxTemp;
            if (faceString.find("//") != std::string::npos)
            {
                //   1//2 2//3 3//4
                for (int i = 0; i < subStrings.size(); i++)
                {
                    std::vector<std::string> indices = splitString(subStrings[i], "//");
                    //std::cout << indices[1] << std::endl;
                    int vertexIndex = std::stoi(indices[0]);
                    if (vertexIndex < 0) vertexIndex = v.size() / 3 + vertexIndex;
                    else vertexIndex -= 1;
                    vidxTemp.push_back(vertexIndex);

                    vertexIndex = std::stoi(indices[1]);
                    if (vertexIndex < 0) vertexIndex = vn.size() / 3 + vertexIndex;
                    else vertexIndex -= 1;
                    vnidxTemp.push_back(vertexIndex);
                }
            }
            else if (faceString.find("/") != std::string::npos)
            {
                //    1/2 2/3 3/4 or 1/2/3 2/3/4 3/4/5
                for (int i = 0; i < subStrings.size(); i++)
                {
                    std::vector<std::string> indices = splitString(subStrings[i], "/");
                    if (indices.size() == 0) std::cout << faceString << std::endl;
                    int vertexIndex = std::stoi(indices[0]);
                    if (vertexIndex < 0) vertexIndex = v.size() / 3 + vertexIndex;
                    else vertexIndex -= 1;
                    vidxTemp.push_back(vertexIndex);

                    if (indices.size() == 2)
                    {
                        vnidxTemp.push_back(-1); // no normals
                    }
                    else if (indices.size() == 3)
                    {
                        vertexIndex = std::stoi(indices[2]);
                        if (vertexIndex < 0) vertexIndex = vn.size() / 3 + vertexIndex;
                        else vertexIndex -= 1;
                        vnidxTemp.push_back(vertexIndex);
                    }
                }
            }
            else
            {
                for (int i = 0; i < subStrings.size(); i++)
                {
                    int vertexIndex = std::stoi(subStrings[i]);
                    if (vertexIndex < 0) vertexIndex = v.size() / 3 + vertexIndex;
                    else vertexIndex -= 1;
                    vidxTemp.push_back(vertexIndex);
                    vnidxTemp.push_back(-1);
                }
            }
            for (int i = 1; i <= subStrings.size() - 2; ++i)
            {
                // triangulate
                vidx.push_back(vidxTemp[0]);
                vidx.push_back(vidxTemp[i]);
                vidx.push_back(vidxTemp[i + 1]);
                vnidx.push_back(vnidxTemp[0]);
                vnidx.push_back(vnidxTemp[i]);
                vnidx.push_back(vnidxTemp[i + 1]);
            }

        }

    }

    fclose(file);

    float xMax = -99999999.0;
    float yMax = -99999999.0;
    float zMax = -99999999.0;
    float xMin = 99999999.0;
    float yMin = 99999999.0;
    float zMin = 99999999.0;
    double xCenter = 0.0;
    double yCenter = 0.0;
    double zCenter = 0.0;
    for (int i = 0; i < v.size() - 2; i += 3)
    {
        xCenter += v[i];
        yCenter += v[i + 1];
        zCenter += v[i + 2];
        if (v[i] > xMax) xMax = v[i];
        if (v[i + 1] > yMax) yMax = v[i + 1];
        if (v[i + 2] > zMax) zMax = v[i + 2];
        if (v[i] < xMin) xMin = v[i];
        if (v[i + 1] < yMin) yMin = v[i + 1];
        if (v[i + 2] < zMin) zMin = v[i + 2];
    }

    xCenter /= (v.size() / 3);
    yCenter /= (v.size() / 3);
    zCenter /= (v.size() / 3);

    for (int i = 0; i < v.size() - 2; i += 3)
    {
        v[i] -= xCenter;
        v[i + 1] -= yCenter;
        v[i + 2] -= zCenter;
    }

    float maxLength = std::max({ xMax - xMin, yMax - yMin, zMax - zMin });

    m_Scale = 4. / maxLength;

    const int n = vidx.size(); // total vertices;
    std::vector<float> vertexbuffer(6 * n);
    for (int i = 0; i < n; i++)
    {
        vertexbuffer[i * 6 + 0] = v[vidx[i] * 3 + 0];
        vertexbuffer[i * 6 + 1] = v[vidx[i] * 3 + 1];
        vertexbuffer[i * 6 + 2] = v[vidx[i] * 3 + 2];
        if (vnidx[i] >= 0)
        {
            vertexbuffer[i * 6 + 3] = vn[vnidx[i] * 3 + 0];
            vertexbuffer[i * 6 + 4] = vn[vnidx[i] * 3 + 1];
            vertexbuffer[i * 6 + 5] = vn[vnidx[i] * 3 + 2];
        }
        else
        {
            vertexbuffer[i * 6 + 3] = 0.;
            vertexbuffer[i * 6 + 4] = 0.;
            vertexbuffer[i * 6 + 5] = 0.;
        }
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

void Object::ResetColor()
{
    m_Color = glm::vec3(1., 1., 1.);
}