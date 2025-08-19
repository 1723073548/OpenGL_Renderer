#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

#include "../Tools/SingletonTemplate.h"
#include "../Camera/Camera.h"

using RenderFunc = std::function<void()>;
using ImGuiFunc = std::function<void()>;

// Callback Functions
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int mouse, int action, int mods);
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);

class WindowMrg : public Singleton<WindowMrg> {
	friend class Singleton<WindowMrg>;

public: // Constractors
	WindowMrg();
	~WindowMrg();
	WindowMrg(const WindowMrg& other) = delete;
	WindowMrg& operator=(const WindowMrg& other) = delete;

public: // Public-Fields
	GLFWwindow* m_glfwWnd;
	std::vector<ImGuiFunc> m_imguiFuncs;
	glm::vec4 m_backgroundColor;
	const char* m_title;
	int m_width;
	int m_height;

public: // Public-Methods
	bool ShouldClose();
	void UpdateWindow(RenderFunc renderFunc);
};