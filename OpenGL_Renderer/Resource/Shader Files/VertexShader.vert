#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out mat4 modelMat;

uniform mat4 modelTrans;
uniform mat4 viewTrans;
uniform mat4 projTrans;

void main()
{
	FragPos = vec3(modelTrans * vec4(aPos, 1.0));

	modelMat = modelTrans;

	Normal = mat3(modelTrans) * aNormal;

	TexCoord = aTexCoords;

	gl_Position = projTrans * viewTrans * modelTrans * vec4(aPos, 1.0);
}