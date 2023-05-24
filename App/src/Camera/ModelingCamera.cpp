#include "ModelingCamera.h"

ModelingCamera::ModelingCamera()
{
	Reset();
}

void ModelingCamera::Reset()
{
	m_horizontalAngle = 112.5f;
	m_verticalAngle = 22.5f;
	m_FoV = 45.0f;
	m_Distance = 15.f;
	m_SpeedHori = 20.f;
	m_SpeedVerti = 20.f;
	m_ScrollSpeed = 1.f;
	b_left = true;

	b_selected = false;
}

void ModelingCamera::Scroll(float offset)
{	
	if (offset > 0 && m_Distance <= 1)
	{

	}
	else if (offset < 0 && m_Distance >= 50)
	{

	}
	else m_Distance -= m_ScrollSpeed * offset;
}

void ModelingCamera::OnUpdate(float timestep, float width, float height)
{
	float xpos = Hazel::Input::GetMouseX();
	float ypos = Hazel::Input::GetMouseY();
	if (!Hazel::Input::IsMouseButtonPressed(Hazel::Mouse::ButtonLeft)) b_selected = false;
	float xposDelta = 0, yposDelta = 0;
	if (b_selected)
	{
		xposDelta = xpos - m_xposPrevious;
		yposDelta = ypos - m_yposPrevious;
	}
	m_xposPrevious = xpos;
	m_yposPrevious = ypos;


	double verticalAngleNew = m_verticalAngle + m_SpeedVerti * timestep * yposDelta;
	verticalAngleNew = fmod(verticalAngleNew, 360.);

	double horizontalAngleNew = (m_verticalAngle > 90 && m_verticalAngle <= 270)? m_horizontalAngle - m_SpeedHori * timestep * xposDelta : m_horizontalAngle + m_SpeedHori * timestep * xposDelta;
	horizontalAngleNew = fmod(horizontalAngleNew, 360.);

	m_verticalAngle = verticalAngleNew;
	m_horizontalAngle = horizontalAngleNew;

	glm::mat4 rot1 = glm::rotate(glm::mat4(1.0), glm::radians((float)m_horizontalAngle), glm::vec3(0, 1, 0));
	glm::mat4 rot2 = glm::rotate(glm::mat4(1.0), glm::radians((float)-m_verticalAngle), glm::vec3(0, 0, 1));
	m_Matrices.CameraModelMatrix = rot2 * rot1 * glm::mat4(1.0);

	if (height == 0.) height = 0.00001f;
	m_Matrices.ProjectionMatrix = glm::perspective(glm::radians((float)m_FoV), (float)width / (float)height, 0.1f, 100.0f);

	m_Matrices.ViewMatrix = glm::lookAt(
		glm::vec3((float)m_Distance, 0, 0),           // Camera is here
		glm::vec3(0, 0, 0), // and looks here : at the same position, plus "direction"
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}