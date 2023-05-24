#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1);
	fragmentColor = vertexColor;
};

#shader fragment
#version 330 core

in vec3 fragmentColor;
out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	color = vec4(fragmentColor, 1);
};