#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1);
};

#shader fragment
#version 330 core

out vec4 color;

uniform vec3 Color;

void main()
{
	color = vec4(Color, 1);
};