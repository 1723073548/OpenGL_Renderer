#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../Shader/Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public: // Constructors
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh(float vertices[]);

public: // Public-Fields
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices; // EBO
	std::vector<Texture> textures;
	unsigned int VAO;

private: // Private-Methods
	unsigned int VBO, EBO;

public: // Public-Methods
	void Draw(Shader shader);

private: // Private-Methods
	void setupMesh();
};