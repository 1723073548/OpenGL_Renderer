#include "Light.h"

Light::Light(
	LightType type,
	glm::vec3 dir,
	glm::vec3 color,
	float intensity,
	glm::vec3 pos,
	float constant,
	float linear,
	float quadratic,
	float cutOff,
	float outerCutOff)
	:
	m_type(type),
	m_dir(dir),
	m_color(color),
	m_intensity(intensity),
	m_pos(pos),
	m_cutOff(cutOff),
	m_outerCutOff(outerCutOff),
	m_constant(constant),
	m_linear(linear),
	m_quadratic(quadratic)
{

}

void Light::SetLight(std::shared_ptr<Shader> shader) {
	switch (m_type) {
	case Light::Directlight: {
		(*shader).setVec3("dirLight.direction", m_dir);
		(*shader).setVec3("dirLight.color", m_color);
		(*shader).setFloat("dirLight.intensity", m_intensity);
		break;
	}
	case Light::Pointlight: {
		(*shader).setVec3("ptLight.position", m_pos);
		(*shader).setVec3("ptLight.color", m_color);
		(*shader).setFloat("ptLight.indensity", m_intensity);
		(*shader).setFloat("ptLight.constant", m_constant);
		(*shader).setFloat("ptLight.linear", m_linear);
		(*shader).setFloat("ptLight.quadratic", m_quadratic);
		break;
	}
	case Light::Spotlight: {
		(*shader).setVec3("spotLight.position", m_pos);
		(*shader).setVec3("spotLight.direction", m_dir);
		(*shader).setVec3("spotLight.color", m_color);
		(*shader).setFloat("spotLight.indensity", m_intensity);
		(*shader).setFloat("spotLight.cutOff", m_cutOff);
		(*shader).setFloat("spotLight.outerCutOff", m_outerCutOff);
		(*shader).setFloat("spotLight.constant", m_constant);
		(*shader).setFloat("spotLight.linear", m_linear);
		(*shader).setFloat("spotLight.quadratic", m_quadratic);
		break;
	}
	default:
		break;
	}
}
