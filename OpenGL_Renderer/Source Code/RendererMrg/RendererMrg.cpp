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

#pragma region Scene/Window Setup
    struct Material {
        glm::vec3 ambientColor = glm::vec3(0.1f);
        glm::vec3 diffuseColor = glm::vec3(1, 1, 1);
        glm::vec3 specularColor = glm::vec3(1, 1, 1);
        float gloss = 0.8f;
    } material;

    // Init Scene Object
	std::shared_ptr<Light> light = std::make_shared<Light>(Light::LightType::Directlight, glm::vec3(-1.0f, -1.0f, 1.0f));
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(
        "Resource/Shader Files/None Texture/VertexShader.vert",
        "Resource/Shader Files/None Texture/FragmentShader.frag");
    std::shared_ptr<Model> teapot = std::make_shared<Model>("Resource/Model/teapot.obj", shader);
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0, 2.5f, 10), 0, -90, glm::vec3(0, 1, 0));

    // Add Object To Scene
    m_mainScene.AddLight(light);
    m_mainScene.AddModel(teapot);
    m_mainScene.SetCamera(camera);

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
            teapot.get()->m_transform.GetPosition().x,
            teapot.get()->m_transform.GetPosition().y,
            teapot.get()->m_transform.GetPosition().z
        };
        ImGui::DragFloat3("Position", pos, 0.1f);
		teapot.get()->m_transform.SetPosition(glm::vec3(pos[0], pos[1], pos[2]));
        ImGui::End();
	};

	WindowMrg::GetInstance().m_imguiFuncs.push_back(guiPanel_1);
	WindowMrg::GetInstance().m_imguiFuncs.push_back(guiPanel_2);
    WindowMrg::GetInstance().m_imguiFuncs.push_back(guiPanel_3);

#pragma endregion

    // Main Loop
	while (!WindowMrg::GetInstance().ShouldClose()) {
        WindowMrg::GetInstance().UpdateWindow(
            [&]() { 
                // Render Function

                // Change Material
                teapot.get()->m_shader->setVec3("material.ambientColor", material.ambientColor);
                teapot.get()->m_shader->setVec3("material.diffuseColor", material.diffuseColor);
                teapot.get()->m_shader->setVec3("material.specularColor", material.specularColor);
                teapot.get()->m_shader->setFloat("material.gloss", material.gloss);

                // Render Scene
                m_mainScene.Render();
            }
        );
	}
	return 0;
}