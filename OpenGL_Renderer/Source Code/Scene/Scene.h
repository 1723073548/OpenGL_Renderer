#pragma once
#include <memory>

#include "../Model/Model.h"
#include "../Light/Light.h"
#include "../Camera/Camera.h"

class Scene {
public: // Constructor
	Scene();
	~Scene() = default;

private: // Private-Fields
	std::vector<std::shared_ptr<Model>> m_models;		 // 模型列表
	std::vector<std::shared_ptr<Light>> m_lights;		 // 光源列表
	std::shared_ptr<Camera> m_mainCamera;				 // 相机

public: // Public-Methods
	void AddModel(std::shared_ptr<Model> model);
	void AddLight(std::shared_ptr<Light> light);
	void SetCamera(std::shared_ptr<Camera> camera);
	void Update(float deltaTime);
	void Render();
};