#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "../Shader/Shader.h"

class Light {
public:
	enum LightType {
		Directlight,
		Pointlight,
		Spotlight
	};

public: // Constrauctors
	Light(
		LightType type = LightType::Directlight,
		glm::vec3 dir = glm::vec3(0),
		glm::vec3 color = glm::vec3(1),
		float intensity = 1.0f,
		glm::vec3 pos = glm::vec3(0),
		float constant = 1.0f,
		float linear = 0.1f,
		float quadratic = 0.032f,
		float cutOff = glm::cos(glm::radians(12.5f)),
		float outerCutOff = glm::cos(glm::radians(17.5f)));
public: // Public-Fields
	LightType m_type;

	// Light Params
	glm::vec3 m_pos;
	glm::vec3 m_dir;
	glm::vec3 m_color;
	float m_intensity;

	// Spot Light Params
	float m_cutOff;				// 内角度
	float m_outerCutOff;		// 外角度

	// Spot/Direct Light Params
	float m_constant;			 // 常数衰减
	float m_linear;				 // 线性衰减
	float m_quadratic;			 // 二次衰减

public: // Public-Methods
	void SetLight(std::shared_ptr<Shader> shader);
};