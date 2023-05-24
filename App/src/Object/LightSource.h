#pragma once
#include "Hazel.h"
#include "Scene/Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class LightSource
{
public:
	LightSource();
	void Init();

	glm::vec3 LightPos;
	glm::vec3 LightColor;
	void ResetLoc();
	void Draw(std::shared_ptr<Hazel::Shader> shader, CameraMatrices matrices, bool left);

private:
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hazel::VertexBufferLayout> m_Layout;
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;
};