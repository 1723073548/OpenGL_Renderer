#version 330 core

uniform struct PointLight
{
	vec3 position;
	vec3 color;
	float indensity;
	float constant;
	float linear;
	float quadratic;
}ptLight;

uniform struct SoptLight
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float indensity;

	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;
}spotLight;

uniform struct DirectLight
{
	vec3 direction;
	vec3 color;
	float indensity;
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

vec3 HandleSpotLight(SoptLight light, vec3 diffuseTex, vec3 specularMask)
{
	// Basic Info
	vec3 N = normalize(Normal);
	vec3 L = normalize(light.position - FragPos);
	vec3 R = normalize(reflect(-L, N));
	vec3 V = normalize(cameraPos - FragPos);
	vec3 H = normalize(L + V);

	// Light Model
	float Lambert = clamp(dot(N, L), 0.0, 1.0);
	float SpecularExponent = exp2(material.gloss * 11) + 2;
	//float Phong = pow(max(dot(V, R), 0.0), SpecularExponent) * material.gloss;
	float Blinn_Phong = pow(clamp(dot(N, H), 0, 1), SpecularExponent) * material.gloss;

	// Spost Light
	float theta = dot(L, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float indensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float Dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * Dist + light.quadratic * (Dist * Dist));

	vec3 Diffuse = diffuseTex * Lambert * light.color * light.indensity * material.diffuseColor * attenuation * indensity;
	vec3 Specular = specularMask * Blinn_Phong * light.color * light.indensity * material.specularColor * attenuation * indensity;

	return vec3(Diffuse + Specular);
}

vec3 HandlePointLight(PointLight light, vec3 diffuseTex, vec3 specularMask)
{
	// Basic Info
	vec3 N = normalize(Normal);
	vec3 L = normalize(light.position - FragPos);
	vec3 R = normalize(reflect(-L, N));
	vec3 V = normalize(cameraPos - FragPos);
	vec3 H = normalize(L + V);

	// Light Model
	float Lambert = clamp(dot(N, L), 0.0, 1.0);
	float SpecularExponent = exp2(material.gloss * 11) + 2;
	
	//float Phong = pow(max(dot(V, R), 0.0), SpecularExponent) * material.gloss;
	float Blinn_Phong = pow(clamp(dot(N, H), 0, 1), SpecularExponent) * material.gloss;
	
	float Dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * Dist + light.quadratic * (Dist * Dist));
	
	vec3 Diffuse = diffuseTex * Lambert * light.color * light.indensity * material.diffuseColor * attenuation;
	vec3 Specular = specularMask * Blinn_Phong * light.color * light.indensity * material.specularColor * attenuation;
	
	return vec3(Diffuse + Specular);
}

vec3 HandleDirectLight(DirectLight light, vec3 diffuseTex, vec3 specularMask) {
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
	
	vec3 Diffuse = diffuseTex * Lambert * light.color * light.indensity * material.diffuseColor;
	vec3 Specular = specularMask * Blinn_Phong * light.color * light.indensity * material.specularColor;

	return vec3(Diffuse + Specular);
}

void main() {
	// Texture
	vec4 TexColor_1 = texture(material.texture_diffuse, TexCoord);
	vec4 TexColor_2 = texture(material.texture_specular, TexCoord);
	vec3 spotLightValue = HandleSpotLight(spotLight, TexColor_1.xyz, TexColor_2.xyz);
	vec3 pointLightValue = HandlePointLight(ptLight, TexColor_1.xyz, TexColor_2.xyz);
	//vec3 directLightValue = HandleDirectLight(dirLight, TexColor_1.xyz, TexColor_2.xyz);
	
	color = vec4(spotLightValue + pointLightValue, 1.0);
}