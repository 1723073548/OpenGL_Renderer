#pragma once
#define GLEW_STATIC
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Model/Model.h"
#include "../Light/Light.h"
#include "../Camera/Camera.h"
#include "../Tools/SingletonTemplate.h"

class MainScene : public Singleton<MainScene> {
	friend class Singleton<MainScene>;
public:
	MainScene();
	~MainScene() = default;
	MainScene(const MainScene& other) = delete;
	MainScene& operator=(const MainScene& other) = delete;

public:
	std::vector<std::shared_ptr<Model>> m_Models;
	std::vector<std::shared_ptr<Light>> m_Lights;
	std::shared_ptr<Camera> m_Camera;

	GLFWwindow* m_glfwWnd;
	glm::vec4 m_backgroundColor;
	const char* m_title;
	int m_width;
	int m_height;
	
public:
	int Init();
	int Render();
	bool ShouldClose();
	void ToggleFullScreen();
	void ToggleShowCursor();
	void ProcessInput();
};