#pragma once
#include <vector>
#include <string>

#include "../Shader/Shader.h"

class Skybox {
public:
    Skybox(std::vector<std::string>faces, Shader shader);
    ~Skybox() = default;

public:
    void Render(glm::mat4 viewTrans, glm::mat4 projTrans);

private:
    Shader m_shader;
    unsigned int m_VAO, m_VBO;
    unsigned int m_textureID;
    unsigned int LoadCubemap(std::vector<std::string> faces);
};