#pragma once
#include "Hazel.h"
#include "hzpch.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera/ModelingCamera.h"
#include "Object/LightSource.h"
#include "Object/Object.h"

class LightSource;
class Object;

class Scene
{
public:
	Scene();
	void Draw();
	void OnUpdate(CameraMatrices matrices, bool left);
	std::shared_ptr<LightSource> m_LightSource;
	std::shared_ptr<Object> m_Object;

private:
	void DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);
	void DrawGrid(int range = 100);

	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::Shader> m_FlatShader;
	std::shared_ptr<Hazel::Shader> m_DiffuseShader;

	CameraMatrices m_Matrices;
	bool b_left;

	std::vector<float> m_CorVertex;
	std::vector<float> m_CorColor;
	std::vector<unsigned int> m_CorIndice;

	
	glm::vec3 m_LightPos;
};