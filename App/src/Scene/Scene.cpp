#include "Scene.h"

//Hazel::Shader m_Shader("assets/shaders/Basic.glsl");

Scene::Scene()
{
	m_Shader = std::make_shared<Hazel::Shader>("assets/shaders/Basic.glsl");
	m_FlatShader = std::make_shared<Hazel::Shader>("assets/shaders/FlatShader.glsl");
	m_DiffuseShader = std::make_shared<Hazel::Shader>("assets/shaders/DiffuseAmbient.glsl");

	m_Object = std::make_shared<Object>();
	m_LightSource = std::make_shared<LightSource>();

	m_Object->ReadFile("assets/objs/bunny.obj");

	int range = 100;
	for (int i = -range; i <= range; ++i)
	{
	
		m_CorVertex.push_back(-range);
		m_CorVertex.push_back(-1.);
		m_CorVertex.push_back(i);
		m_CorVertex.push_back(range);
		m_CorVertex.push_back(-1.);
		m_CorVertex.push_back(i);

		m_CorVertex.push_back(i);
		m_CorVertex.push_back(-1.);
		m_CorVertex.push_back((float)-range);
		m_CorVertex.push_back(i);
		m_CorVertex.push_back(-1.);
		m_CorVertex.push_back((float)range);

		for (int j = 0; j < 12; ++j)
			m_CorColor.push_back(0.3);

		
	}
	for (int i = 0; i < m_CorVertex.size() / 3; i++)
		m_CorIndice.push_back(i);

}

void Scene::OnUpdate(CameraMatrices matrices, bool left)
{
	m_Matrices = matrices;
	b_left = left;
}


void Scene::Draw()
{	
	//DrawLine(-100, -1, 0, 100, -1, 0, 1, 0, 0);
	//DrawLine(0, -1, 100, 0, -1, -100, 0, 1, 0);
	//DrawGrid();

	m_FlatShader->Bind();
	m_LightSource->Draw(m_FlatShader, m_Matrices, b_left);

	m_DiffuseShader->Bind();
	//glm::vec3 LightPos = m_Matrices.CameraModelMatrix * glm::vec4(m_LightSource->LightPos, 1.);
	glm::vec3 LightPos = glm::vec4(m_LightSource->LightPos, 1.);
	m_DiffuseShader->SetUniform3f("LightPos", LightPos[0], LightPos[1], LightPos[2]);
	m_DiffuseShader->SetUniform3f("LightColor", m_LightSource->LightColor[0], m_LightSource->LightColor[1], m_LightSource->LightColor[2]);
	m_Object->Draw(m_DiffuseShader, m_Matrices, b_left);
}

void Scene::DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
{
	std::vector<float> LineVertex;
	std::vector<float> LineColor;
	std::vector<unsigned int> LineIndice;
	int range = 100;
	LineVertex.push_back(x1);
	LineVertex.push_back(y1);
	LineVertex.push_back(z1);
	LineVertex.push_back(x2);
	LineVertex.push_back(y2);
	LineVertex.push_back(z2);

	LineColor.push_back(r);
	LineColor.push_back(g);
	LineColor.push_back(b);
	LineColor.push_back(r);
	LineColor.push_back(g);
	LineColor.push_back(b);

	LineIndice.push_back(0);
	LineIndice.push_back(1);

	Hazel::VertexArray vaLine;
	Hazel::VertexBuffer vbLineVertex(&LineVertex[0], 6 * sizeof(float));
	Hazel::VertexBuffer vbLineColor(&LineColor[0], 6 * sizeof(float));
	Hazel::VertexBufferLayout LineVertexLayout;
	Hazel::VertexBufferLayout LineColorLayout;
	Hazel::IndexBuffer idLine(&LineIndice[0], 2);
	LineVertexLayout.Push<float>(3);
	LineColorLayout.Push<float>(3);
	vaLine.AddBuffer(vbLineVertex, LineVertexLayout);
	vaLine.AddBuffer(vbLineColor, LineColorLayout);

	glm::mat4 ModelMatrix = b_left ? m_Matrices.CameraModelMatrix * glm::mat4(1.0) : glm::mat4(1.0) * m_Matrices.CameraModelMatrix;

	m_Shader->SetUniformMatrix4fv("ProjectionMatrix", &m_Matrices.ProjectionMatrix[0][0]);
	m_Shader->SetUniformMatrix4fv("ViewMatrix", &m_Matrices.ViewMatrix[0][0]);
	m_Shader->SetUniformMatrix4fv("ModelMatrix", &ModelMatrix[0][0]);

	Hazel::Renderer::DrawLine(vaLine, idLine, *m_Shader);
}


void Scene::DrawGrid(int range)
{

	Hazel::VertexArray vaLine;
	Hazel::VertexBuffer vbLineVertex(&m_CorVertex[0], m_CorVertex.size() * sizeof(float));
	Hazel::VertexBuffer vbLineColor(&m_CorColor[0], m_CorColor.size() * sizeof(float));
	Hazel::VertexBufferLayout LineVertexLayout;
	Hazel::VertexBufferLayout LineColorLayout;
	Hazel::IndexBuffer idLine(&m_CorIndice[0], m_CorIndice.size());
	LineVertexLayout.Push<float>(3);
	LineColorLayout.Push<float>(3);
	vaLine.AddBuffer(vbLineVertex, LineVertexLayout);
	vaLine.AddBuffer(vbLineColor, LineColorLayout); 

	glm::mat4 ModelMatrix = b_left ? m_Matrices.CameraModelMatrix * glm::mat4(1.0) : glm::mat4(1.0) * m_Matrices.CameraModelMatrix;

	m_Shader->Bind();
	m_Shader->SetUniformMatrix4fv("ProjectionMatrix", &m_Matrices.ProjectionMatrix[0][0]);
	m_Shader->SetUniformMatrix4fv("ViewMatrix", &m_Matrices.ViewMatrix[0][0]);
	m_Shader->SetUniformMatrix4fv("ModelMatrix", &ModelMatrix[0][0]);

	Hazel::Renderer::DrawLine(vaLine, idLine, *m_Shader);


}