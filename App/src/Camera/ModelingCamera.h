#pragma once
#include "Hazel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct CameraMatrices
{
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 CameraModelMatrix;
};

class ModelingCamera
{
public:
	ModelingCamera();
	void Reset();
	void OnUpdate(float timestep, float width, float height);
	void Scroll(float offset);
	void SetSelected(bool selected) { b_selected = selected; }
	CameraMatrices CameraData() { return m_Matrices; }

private:
	CameraMatrices m_Matrices;
	bool b_selected;
	bool b_left;

	double m_horizontalAngle;
	double m_verticalAngle;
	double m_SpeedHori, m_SpeedVerti;

	float m_FoV;
	float m_Distance;
	float m_xposPrevious, m_yposPrevious;
	float m_ScrollSpeed;
};