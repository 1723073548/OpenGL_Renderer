#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "../Transform/Transform.h"
#include "../Mesh/Mesh.h"
#include "../Shader/Shader.h"
#include "../Light/Light.h"

class Model {
public: // Constructors
	Model(std::string path, Shader shader);

public: // Public-Fields
	Transform m_transform;
	std::vector<Texture> m_texturesLoaded;
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	Shader m_shader;
	bool m_gammaCorrection;

public: // Public Methods
    void Draw(std::function<void()> setShaderParam);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);

private: // Private Methods 
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

