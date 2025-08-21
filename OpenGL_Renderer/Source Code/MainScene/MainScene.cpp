#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MainScene.h"

#include "../Skybox/Skybox.h"

#include "../Thirdparty/imgui/imgui.h"
#include "../Thirdparty/imgui/imgui_impl_glfw.h"
#include "../Thirdparty/imgui/imgui_impl_opengl3.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int mouse, int action, int mods);
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);

MainScene::MainScene()
	:
	m_width(1920),
	m_height(1080),
	m_title("OpenGL Window"),
	m_glfwWnd(nullptr),
	m_backgroundColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
{
	
}

int MainScene::Init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);  // 禁用窗口最小化时自动暂停

	m_glfwWnd = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

	if (m_glfwWnd == NULL) {
		glfwTerminate();
		std::cout << "Failed to create GLFW window" << std::endl;
		return 0;
	}

	glfwSwapInterval(1);

	// Init Callback
	glfwSetKeyCallback(m_glfwWnd, KeyCallback);
	glfwSetMouseButtonCallback(m_glfwWnd, MouseButtonCallback);
	glfwSetFramebufferSizeCallback(m_glfwWnd, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(m_glfwWnd, CursorPosCallback);
	glfwSetScrollCallback(m_glfwWnd, ScrollCallback);

	glfwMakeContextCurrent(m_glfwWnd);

	if (glewInit() != GLEW_OK) {
		printf("Init GLEW failed.");
		glfwTerminate();
		return 0;
	}

	// Init imGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_glfwWnd, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	return 1;
}

int MainScene::Render() {
	if (!Init()) return 0;

#pragma region Resource Setup
    struct Material {
        glm::vec3 ambientColor = glm::vec3(0.1f);
        glm::vec3 diffuseColor = glm::vec3(1, 1, 1);
        glm::vec3 specularColor = glm::vec3(1, 1, 1);
        float gloss = 0.8f;
    } material;

    Light dirLight(Light::LightType::Directlight, glm::vec3(-1.0f, -1.0f, 1.0f));

    Shader teapotShader(
        "Resource/Shader Files/None Texture/VertexShader.vert",
        "Resource/Shader Files/None Texture/FragmentShader.frag");

    Model teapot("Resource/Model/teapot.obj", teapotShader);

    Camera camera(
        glm::vec3(0, 1.5f, 5), // Position
        0,                     // Pitch
        -90,                   // Yaw
        glm::vec3(0, 1, 0));   // World Up

    Skybox skybox(
        std::vector<std::string>{
        "Resource/Picture/Skybox/right.jpg",
            "Resource/Picture/Skybox/left.jpg",
            "Resource/Picture/Skybox/top.jpg",
            "Resource/Picture/Skybox/bottom.jpg",
            "Resource/Picture/Skybox/front.jpg",
            "Resource/Picture/Skybox/back.jpg"
    },
        Shader("Resource/Shader Files/Skybox/VertexShader.vert", "Resource/Shader Files/Skybox/FragmentShader.frag")
    );

    bool showSkybox = true;

	MainScene::GetInstance().m_Camera = std::make_shared<Camera>(camera);

#pragma region imGUI
    std::function<void()> guiPanel_1 = [&]() {
        ImGui::Begin("Material Settings");
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(material.ambientColor));
        ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(material.diffuseColor));
        ImGui::ColorEdit3("Specular Color", glm::value_ptr(material.specularColor));
        ImGui::SliderFloat("Gloss", &material.gloss, 0.0f, 1.0f);
        ImGui::End();
        };

    std::function<void()> guiPanel_2 = [&]() {
        ImGui::Begin("Background Settings");
        float backgroundColor[4] = {
            m_backgroundColor.r,
            m_backgroundColor.g,
            m_backgroundColor.b,
            m_backgroundColor.a
        };

        ImGui::ColorEdit4("Background Color", backgroundColor);

        m_backgroundColor = glm::vec4(
            backgroundColor[0],
            backgroundColor[1],
            backgroundColor[2],
            backgroundColor[3]
        );
        ImGui::End();
        };

    std::function<void()> guiPanel_3 = [&]() {
        ImGui::Begin("Model Settings");
        float pos[3] = {
            teapot.m_transform.GetPosition().x,
            teapot.m_transform.GetPosition().y,
            teapot.m_transform.GetPosition().z
        };
        ImGui::DragFloat3("Position", pos, 0.1f);
        teapot.m_transform.SetPosition(glm::vec3(pos[0], pos[1], pos[2]));
        ImGui::End();
        };

    std::function<void()> guiPanel_4 = [&]() {
        ImGui::Begin("Sky Settings");
        if (ImGui::Checkbox("Show Skybox", &showSkybox))
            ImGui::End();
        };
#pragma endregion

#pragma endregion

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);

    // Main Loop
    while (!ShouldClose()) {
		ProcessInput();
		 
        glm::mat4 viewTrans, projTrans;
        viewTrans = m_Camera.get()->GetViewMatrix();
        projTrans = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(m_width) / static_cast<float>(m_height),
            0.1f,
            100.0f);

        glClearColor(
			m_backgroundColor.r,
			m_backgroundColor.g,
			m_backgroundColor.b,
			m_backgroundColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        teapot.Draw([&]() {
            teapotShader.setMat4("modelTrans", teapot.m_transform.GetModelMatrix());
            teapotShader.setMat4("viewTrans", viewTrans);
            teapotShader.setMat4("projTrans", projTrans);
            teapotShader.setVec3("cameraPos", m_Camera.get()->Position);
            teapotShader.setVec3("material.ambientColor", material.ambientColor);
            teapotShader.setVec3("material.diffuseColor", material.diffuseColor);
            teapotShader.setVec3("material.specularColor", material.specularColor);
            teapotShader.setFloat("material.gloss", material.gloss);
            dirLight.SetLight(teapot.m_shader);
            });

        if (showSkybox) {
            skybox.Render(
                glm::mat4(glm::mat3(viewTrans)),
                projTrans);
        }

#pragma region Handle imGUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        guiPanel_1();
        guiPanel_2();
        guiPanel_3();
        guiPanel_4();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_glfwWnd);
        glfwPollEvents();
#pragma endregion
    }
    return 1;
}

bool MainScene::ShouldClose() {
	return glfwWindowShouldClose(m_glfwWnd);
}

void MainScene::ToggleFullScreen() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	// 保存窗口模式的位置和大小（只在第一次进入全屏时保存）
	static int savedWindowPos[2] = { 0, 0 };
	static int savedWindowSize[2] = { m_width, m_height };

	if (glfwGetWindowMonitor(m_glfwWnd) == nullptr) {
		// 保存当前窗口位置和大小
		glfwGetWindowPos(m_glfwWnd, &savedWindowPos[0], &savedWindowPos[1]);
		glfwGetWindowSize(m_glfwWnd, &savedWindowSize[0], &savedWindowSize[1]);

		// 计算保持宽高比的全屏尺寸
		float windowAspect = (float)savedWindowSize[0] / (float)savedWindowSize[1];
		float monitorAspect = (float)mode->width / (float)mode->height;

		int fullWidth, fullHeight;

		if (windowAspect > monitorAspect) {
			// 窗口比显示器宽，以宽度为准
			fullWidth = mode->width;
			fullHeight = (int)(mode->width / windowAspect);
		}
		else {
			// 窗口比显示器高，以高度为准
			fullHeight = mode->height;
			fullWidth = (int)(mode->height * windowAspect);
		}

		// 进入全屏模式，保持宽高比
		glfwSetWindowMonitor(
			m_glfwWnd, monitor,
			(mode->width - fullWidth) / 2,  // 水平居中
			(mode->height - fullHeight) / 2, // 垂直居中
			fullWidth, fullHeight,
			mode->refreshRate
		);
	}
	else {
		// 退出全屏模式，恢复之前的位置和大小
		glfwSetWindowMonitor(
			m_glfwWnd, nullptr,
			savedWindowPos[0], savedWindowPos[1],
			savedWindowSize[0], savedWindowSize[1],
			mode->refreshRate
		);
	}

	// 更新视口
	int width, height;
	glfwGetFramebufferSize(m_glfwWnd, &width, &height);
	glViewport(0, 0, width, height);
}

void MainScene::ToggleShowCursor() {
	if (glfwGetInputMode(m_glfwWnd, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		glfwSetInputMode(m_glfwWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 显示光标
	}
	else {
		glfwSetInputMode(m_glfwWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 关闭光标
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		MainScene::GetInstance().ToggleFullScreen();
	}
}

void MouseButtonCallback(GLFWwindow* window, int mouse, int action, int mods) {
	if (mouse == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		MainScene::GetInstance().m_Camera.get()->ResetFirstMouse();
		MainScene::GetInstance().ToggleShowCursor();
	}
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	MainScene& mainScene = MainScene::GetInstance();

	// 只在窗口模式下更新存储的尺寸
	if (glfwGetWindowMonitor(window) == nullptr) {
		mainScene.m_width = width;
		mainScene.m_height = height;
	}

	// 计算保持宽高比的视口
	float targetAspect = (float)mainScene.m_width / (float)mainScene.m_height;
	float currentAspect = (float)width / (float)height;

	int viewportWidth, viewportHeight;
	int viewportX = 0, viewportY = 0;

	if (currentAspect > targetAspect) {
		// 窗口比目标宽，以高度为准
		viewportHeight = height;
		viewportWidth = (int)(height * targetAspect);
		viewportX = (width - viewportWidth) / 2;
	}
	else {
		// 窗口比目标高，以宽度为准
		viewportWidth = width;
		viewportHeight = (int)(width / targetAspect);
		viewportY = (height - viewportHeight) / 2;
	}

	// 设置视口，保持宽高比
	glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	MainScene::GetInstance().m_Camera.get()->MoveCamera(glm::vec2(0, yOffset), 0.05f);
}

void CursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	if (glfwGetInputMode(MainScene::GetInstance().m_glfwWnd, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		MainScene::GetInstance().m_Camera.get()->RotateCamera(xPos, yPos);
	}
}

void MainScene::ProcessInput() {
	if (glfwGetInputMode(m_glfwWnd, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {

		glm::vec2 moveDirection = glm::vec2(0, 0);
		if (glfwGetKey(m_glfwWnd, GLFW_KEY_W) == GLFW_PRESS) moveDirection.y += 1;
		if (glfwGetKey(m_glfwWnd, GLFW_KEY_S) == GLFW_PRESS) moveDirection.y -= 1;
		if (glfwGetKey(m_glfwWnd, GLFW_KEY_A) == GLFW_PRESS) moveDirection.x -= 1;
		if (glfwGetKey(m_glfwWnd, GLFW_KEY_D) == GLFW_PRESS) moveDirection.x += 1;
		m_Camera.get()->MoveCamera(moveDirection);
	}
}