#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "shader.h"
#include "texture.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
};

class Mesh {
private:
	// render data. instanced_vbo is the buffer for the instanced matrix
	unsigned int vao, vbo, ebo, instanced_vbo;
	// functions
	void setup_mesh();
	void setup_instanced_mesh(glm::mat4* model_matrices, const unsigned int amount);

public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// functions
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(const Shader& shader);
	void draw_instanced(const Shader& shader, glm::mat4* model_matrices, const unsigned int amount);
};