#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <functional>

#include "../Thirdparty/imGUI/imgui.h"
#include "../Shader/Shader.h"
#include "../Model/Model.h"
#include "../Camera/Camera.h"
#include "../Light/Light.h"
#include "../Skybox/Skybox.h"

#include "../Thirdparty/imgui/imgui.h"
#include "../Thirdparty/imgui/imgui_impl_glfw.h"
#include "../Thirdparty/imgui/imgui_impl_opengl3.h"

#include "RendererMrg.h"

RendererMrg::RendererMrg() {
    WindowMrg::GetInstance(); // Init
}

int RendererMrg::Go() {
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("Init GLEW failed.");
        glfwTerminate();
        return false;
    }

#pragma region Resource Setup
    struct Material {
        glm::vec3 ambientColor = glm::vec3(0.1f);
        glm::vec3 diffuseColor = glm::vec3(1, 1, 1);
        glm::vec3 specularColor = glm::vec3(1, 1, 1);
        float gloss = 0.8f;
    } material;

    // Init Scene Object
    Light light(Light::LightType::Directlight, glm::vec3(-1.0f, -1.0f, 1.0f));
    Shader shader(
        "Resource/Shader Files/None Texture/VertexShader.vert",
        "Resource/Shader Files/None Texture/FragmentShader.frag");
    Model teapot(
        "Resource/Model/teapot.obj",
		std::make_shared<Shader>(shader));
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
        // Convert glm::vec4 to float array for ImGui::ColorEdit4
        float backgroundColor[4] = {
            WindowMrg::GetInstance().m_backgroundColor.r,
            WindowMrg::GetInstance().m_backgroundColor.g,
            WindowMrg::GetInstance().m_backgroundColor.b,
            WindowMrg::GetInstance().m_backgroundColor.a
        };

        ImGui::ColorEdit4("Background Color", backgroundColor);

        // Update glm::vec4 with modified values
        WindowMrg::GetInstance().m_backgroundColor = glm::vec4(
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

    bool showSkybox = true;
    std::function<void()> guiPanel_4 = [&]() {
        ImGui::Begin("Sky Settings");
        if (ImGui::Checkbox("Show Skybox", &showSkybox))
        ImGui::End();
    };
#pragma endregion

#pragma endregion
    glEnable(GL_DEPTH_TEST);
    // Main Loop
	while (!WindowMrg::GetInstance().ShouldClose()) {
        // Change Material
        shader.setVec3("material.ambientColor", material.ambientColor);
        shader.setVec3("material.diffuseColor", material.diffuseColor);
        shader.setVec3("material.specularColor", material.specularColor);
        shader.setFloat("material.gloss", material.gloss);

        // Render Scene
        glm::mat4 modelTrans, viewTrans, projTrans;
        viewTrans = camera.GetViewMatrix();
        projTrans = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(WindowMrg::GetInstance().m_width) / static_cast<float>(WindowMrg::GetInstance().m_height),
            0.1f,
            100.0f);

        // 只在每帧渲染前清除一次缓冲区
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Matrix
        modelTrans = teapot.m_transform.GetModelMatrix();
        shader.setMat4("modelTrans", modelTrans);
        shader.setMat4("viewTrans", viewTrans);
        shader.setMat4("projTrans", projTrans);
        shader.setVec3("cameraPos", camera.Position);
        teapot.Draw(light);

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
        glfwSwapBuffers(WindowMrg::GetInstance().m_glfwWnd);
        glfwPollEvents();
#pragma endregion
	}
	return 0;
}