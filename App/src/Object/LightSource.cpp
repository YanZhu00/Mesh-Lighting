#include "LightSource.h"

LightSource::LightSource()
{
	static const float Positions[] = {
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f
	};

	unsigned int MeshIndice[]
	{
		1, 0, 2,
		0, 2, 3,
		4, 5, 6,
		6, 7, 4,
		2, 1, 5,
		2, 5, 6,
		0, 3, 4,
		3, 4, 7,
		0, 1, 4,
		1, 4, 5,
		2, 3, 6,
		3, 6, 7
	};
	Init();

	m_VertexArray = std::make_shared<Hazel::VertexArray>();
	m_VertexBuffer = std::make_shared<Hazel::VertexBuffer>(Positions, 24 * sizeof(float));
	m_IndexBuffer = std::make_shared<Hazel::IndexBuffer>(MeshIndice, 36);
	m_Layout = std::make_shared<Hazel::VertexBufferLayout>();
	m_Layout->Push<float>(3);
	m_VertexArray->AddBuffer(*m_VertexBuffer, *m_Layout);
}

void LightSource::Init()
{
	LightPos = glm::vec3(3.f, 3.f, 3.f);
	LightColor = glm::vec3(1.f, 1.f, 1.f);
}

void LightSource::ResetLoc()
{
	LightPos = glm::vec3(3.f, 3.f, 3.f);
}



void LightSource::Draw(std::shared_ptr<Hazel::Shader> shader, CameraMatrices matrices, bool left)
{
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, LightPos);
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1, 0.1, 0.1));
	
	//ModelMatrix = left ? matrices.CameraModelMatrix * ModelMatrix : ModelMatrix * matrices.CameraModelMatrix;

	shader->Bind();
	shader->SetUniformMatrix4fv("ProjectionMatrix", &matrices.ProjectionMatrix[0][0]);
	shader->SetUniformMatrix4fv("ViewMatrix", &matrices.ViewMatrix[0][0]);
	shader->SetUniformMatrix4fv("ModelMatrix", &ModelMatrix[0][0]);
	shader->SetUniform3f("Color", LightColor[0], LightColor[1], LightColor[2]);


	Hazel::Renderer::Draw(*m_VertexArray, *m_IndexBuffer, *shader);

}

