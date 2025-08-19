#pragma once
#define GLEW_STATIC
#include <glm/glm.hpp>
#include <string>

class Shader {
public: // Constructor
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader() = default;

public: // Public-Fields
	unsigned int m_ID;

public: // Public-Methods
	void Use();
	void setVec3(const char* parameter, glm::vec3 value);
	void setFloat(const char* parameter, float value);
	void setMat4(const char* parameter, glm::mat4 value);
	void setInt1(const char* parameter, int value);

private: // Private-Methods
	unsigned int CompileShader(unsigned int type, const char* shaderSrc);
	unsigned int CreateShaderProgram(const char* vertexShader, const char* fragmentShder);
	void CheckCompileErrors(unsigned int ID, std::string type);
};