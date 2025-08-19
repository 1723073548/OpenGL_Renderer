#include <stdexcept>

#include "WindowMrg.h"

#include "../Thirdparty/imgui/imgui.h"
#include "../Thirdparty/imgui/imgui_impl_glfw.h"
#include "../Thirdparty/imgui/imgui_impl_opengl3.h"

WindowMrg::WindowMrg()
	:
	m_width(1920),
	m_height(1080),
	m_title("OpenGL Window"),
	m_glfwWnd(nullptr),
	m_backgroundColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);  // 禁用窗口最小化时自动暂停

	m_glfwWnd = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

	if (m_glfwWnd == NULL) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSwapInterval(1);  // 开启垂直同步（避免渲染卡顿）

	// Init Callback
	glfwSetKeyCallback(m_glfwWnd, KeyCallback);
	glfwSetMouseButtonCallback(m_glfwWnd, MouseButtonCallback);
	glfwSetFramebufferSizeCallback(m_glfwWnd, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(m_glfwWnd, CursorPosCallback);
	glfwSetScrollCallback(m_glfwWnd, ScrollCallback);
	glfwMakeContextCurrent(m_glfwWnd);

	// Init imGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_glfwWnd, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

WindowMrg::~WindowMrg() {
	glfwTerminate();
}

bool WindowMrg::ShouldClose() {
	return glfwWindowShouldClose(m_glfwWnd);
}

void WindowMrg::UpdateWindow(RenderFunc renderFunc) {
	glClearColor(
		m_backgroundColor.r,
		m_backgroundColor.g,
		m_backgroundColor.b,
		m_backgroundColor.a);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render Function!
	if (renderFunc) renderFunc();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	for (const auto& guiFunc : m_imguiFuncs) {

		guiFunc();
	}ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(m_glfwWnd);
	glfwPollEvents();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void MouseButtonCallback(GLFWwindow* window, int mouse, int action, int mods) {
	if (mouse == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {

	}
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {

	WindowMrg::GetInstance().m_width = width;
	WindowMrg::GetInstance().m_height = height;
	glViewport(0, 0, width, height);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {

}

void CursorPosCallback(GLFWwindow* window, double xPos, double yPos) {

}