#version 400 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 givenColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 normal;

out vec2 UV;
out vec3 outColor;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	Normal = normal;
	UV = vertexUV;
	FragPos = vec3(model * vec4(vertexPosition, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = MVP * vec4(vertexPosition,1);
	outColor = givenColor;
}