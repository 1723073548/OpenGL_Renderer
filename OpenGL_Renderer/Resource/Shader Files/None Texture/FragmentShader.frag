#version 330 core

uniform struct DirectLight
{
	vec3 direction;
	vec3 color;
	float intensity;
}dirLight;

uniform struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float gloss;
}material;

uniform vec3 cameraPos;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat4 modelMat;

out vec4 color;


vec3 HandleDirectLight(DirectLight light, vec3 diffuseTex, vec3 specularMask)
{
	// Direct Light Model
	vec3 N = normalize(Normal);
	vec3 L = normalize(light.direction);
	vec3 R = normalize(reflect(-L, N));
	vec3 V = normalize(cameraPos - FragPos);
	vec3 H = normalize(L + V);
	
	float Lambert = clamp(dot(N, L), 0.0, 1.0);
	float SpecularExponent = exp2(material.gloss * 11) + 2;

	//float Phong = pow(max(dot(V, R), 0.0), SpecularExponent) * material.gloss;
	float Blinn_Phong = pow(clamp(dot(N, H), 0, 1), SpecularExponent) * material.gloss;
	
	vec3 Diffuse = diffuseTex * Lambert * light.color * light.intensity * material.diffuseColor;
	vec3 Specular = specularMask * Blinn_Phong * light.color * light.intensity * material.specularColor;

	return vec3(Diffuse + Specular);
}

void main()
{
	vec3 directLightValue = HandleDirectLight(dirLight, vec3(1), vec3(1));
	
	color = vec4(directLightValue, 1.0);
}