#include "Scene.h"

#include "../WindowMrg/WindowMrg.h"

Scene::Scene() {

}

void Scene::AddModel(std::shared_ptr<Model> model) {
	m_models.push_back(model);
}

void Scene::AddLight(std::shared_ptr<Light> light) {
	m_lights.push_back(light);
}

void Scene::SetCamera(std::shared_ptr<Camera> camera) {
	m_mainCamera = camera;
}

void Scene::Update(float deltaTime) {

}

void Scene::Render() {
	glm::mat4 modelTrans, viewTrans, projTrans;
	viewTrans = m_mainCamera.get()->GetViewMatrix();
	projTrans = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(WindowMrg::GetInstance().m_width) / static_cast<float>(WindowMrg::GetInstance().m_height),
		0.1f,
		100.0f);

	for (const auto& model : m_models) {
		// Matrix
		modelTrans = model.get()->m_transform.GetModelMatrix();
		model.get()->m_shader->setMat4("modelTrans", modelTrans);
		model.get()->m_shader->setMat4("viewTrans", viewTrans);
		model.get()->m_shader->setMat4("projTrans", projTrans);

		// Camera
		model.get()->m_shader->setVec3("cameraPos", m_mainCamera.get()->Position);

		// Draw Model
		model.get()->Draw(m_lights[0]);
	}
}
