#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	FragPos = vec3(ModelMatrix * vec4(position, 1.0));
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
	Normal = vec3(ModelMatrix * vec4(aNormal, 1.0));
};

#shader fragment
#version 330 core

in vec3 FragPos;  
in vec3 Normal;    
out vec4 color;

uniform mat4 ModelMatrix;
uniform vec3 LightPos;  
uniform vec3 LightColor;  
uniform vec3 ObjectColor;  

vec3 ambientDir = vec3(0.2, 0.0, 0.0);

void main()
{
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(vec3(ModelMatrix * vec4(LightPos, 1.0)) - FragPos);  
	vec3 lightDir = normalize(LightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * LightColor;
	float ambient = max(dot(norm, ambientDir), 0.0);
	vec3 result = (diffuse+ambient) * ObjectColor; 
	color = vec4(result, 1.0);
};