#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::ifstream vertexFile, fragmentFile;
	std::stringstream vertexStringStream, fragmentStringStream;
	std::string vertexString, fragmentString;

	//开裆并不意味着里面的内容被读取到内存中，这里开裆只是提示系统我目前将占用这个档案，以免其他程序对该档案造成修改。
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);

	//保证ifstream可以抛出异常
	vertexFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	try {

		if (!vertexFile.is_open() || !fragmentFile.is_open()) {
			throw std::runtime_error("Open File Error!");
		}

		//File Buffer -> String Buffer
		vertexStringStream << vertexFile.rdbuf();
		fragmentStringStream << fragmentFile.rdbuf();

		//String Buffer -> String
		vertexString = vertexStringStream.str();
		fragmentString = fragmentStringStream.str();

		//String -> const char*
		const char* vertexSrc = vertexString.c_str();
		const char* fragmentSrc = fragmentString.c_str();

		this->m_ID = CreateShaderProgram(vertexSrc, fragmentSrc);
	}
	catch (const std::exception& e) {
		printf(e.what());
	}
}

unsigned int Shader::CompileShader(unsigned int type, const char* shaderSrc) {
	unsigned int shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);

	if (type == GL_VERTEX_SHADER) {
		CheckCompileErrors(shader, "VERTEX");
	}
	else if (type == GL_FRAGMENT_SHADER) {
		CheckCompileErrors(shader, "FRAGMENT");
	}

	return shader;
}

unsigned int Shader::CreateShaderProgram(const char* vertexShader, const char* fragmentShader) {
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);

	glLinkProgram(shaderProgram);
	CheckCompileErrors(shaderProgram, "PROGRAM");

	glUseProgram(shaderProgram);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return shaderProgram;
}

void Shader::CheckCompileErrors(unsigned int ID, std::string type) {
	int success;
	char inforLog[512];

	if (type == "PROGRAM") {
		glGetProgramiv(ID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, inforLog);
			std::cout << "Program Link Error: \n" << inforLog << std::endl;
		}
	}
	else {
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(ID, 512, NULL, inforLog);
			std::cout << type << " Shader Compile Error: \n" << inforLog << std::endl;
		}
	}
}

void Shader::Use() {
	glUseProgram(m_ID);
}

void Shader::setVec3(const char* parameter, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(m_ID, parameter), value[0], value[1], value[2]);
}

void Shader::setFloat(const char* parameter, float value)
{
	glUniform1f(glGetUniformLocation(m_ID, parameter), value);
}

void Shader::setMat4(const char* parameter, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, parameter), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setInt1(const char* parameter, int value)
{
	glUniform1i(glGetUniformLocation(m_ID, parameter), value);
}