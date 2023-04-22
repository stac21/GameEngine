#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setup_mesh();
}

void Mesh::draw(const Shader& shader) {
	if (this->vao == NULL) {
		this->setup_mesh();
	}

	unsigned int diffuse_num = 1;
	unsigned int specular_num = 1;
	unsigned int normal_num = 1;
	unsigned int height_num = 1;
	unsigned int reflection_num = 1;

	for (unsigned int i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		Texture texture = this->textures.at(i);

		std::string num;
		int type = texture.get_type();
		std::string type_string;

		switch (type) {
		case Texture::DIFFUSE:
			num = std::to_string(diffuse_num++);
			type_string = "texture_diffuse";
			break;
		case Texture::SPECULAR:
			num = std::to_string(specular_num++);
			type_string = "texture_specular";
			break;
		case Texture::NORMAL:
			num = std::to_string(normal_num++);
			type_string = "texture_normal";
			break;
		case Texture::HEIGHT:
			num = std::to_string(height_num++);
			type_string = "texture_height";
			break;
		case Texture::REFLECTION:
			num = std::to_string(reflection_num++);
			type_string = "texture_reflection";
			break;
		}

		shader.set_float((type_string + num).c_str(), i);
		glBindTexture(texture.get_target(), texture.get_id());
	}

	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::draw_instanced(const Shader& shader, glm::mat4* model_matrices, const unsigned int amount) {
	this->setup_instanced_mesh(model_matrices, amount);

	unsigned int diffuse_num = 1;
	unsigned int specular_num = 1;
	unsigned int normal_num = 1;
	unsigned int height_num = 1;
	unsigned int reflection_num = 1;

	for (unsigned int i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		Texture texture = this->textures.at(i);

		std::string num;
		int type = texture.get_type();
		std::string type_string;

		switch (type) {
		case Texture::DIFFUSE:
			num = std::to_string(diffuse_num++);
			type_string = "texture_diffuse";
			break;
		case Texture::SPECULAR:
			num = std::to_string(specular_num++);
			type_string = "texture_specular";
			break;
		case Texture::NORMAL:
			num = std::to_string(normal_num++);
			type_string = "texture_normal";
			break;
		case Texture::HEIGHT:
			num = std::to_string(height_num++);
			type_string = "texture_height";
			break;
		case Texture::REFLECTION:
			num = std::to_string(reflection_num++);
			type_string = "texture_reflection";
			break;
		}

		shader.set_float((type_string + num).c_str(), i);
		glBindTexture(texture.get_target(), texture.get_id());
	}

	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(this->vao);
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, amount);
	glBindVertexArray(0);
}

void Mesh::setup_mesh() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/*
	Because the memory layout of structs in C++ is sequential, we can directly pass a pointer
	to a large list of Vertex structs as the buffer's data and they translate perfectly to what
	glBufferData expects as the data argument
	*/
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// texCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::setup_instanced_mesh(glm::mat4* model_matrices, const unsigned int amount) {
	// if the mesh has not already been set up once before, set it up
	if (this->vao == NULL) {
		glGenVertexArrays(1, &this->vao);
		glGenBuffers(1, &this->vbo);
		glGenBuffers(1, &this->ebo);
		glGenBuffers(1, &this->instanced_vbo);
	}

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	/*
	Because the memory layout of structs in C++ is sequential, we can directly pass a pointer
	to a large list of Vertex structs as the buffer's data and they translate perfectly to what
	glBufferData expects as the data argument
	*/
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// texCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	// instanced matrix
	glBindBuffer(GL_ARRAY_BUFFER, this->instanced_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &model_matrices[0], GL_STATIC_DRAW);

	GLsizei vec4_size = sizeof(glm::vec4);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(vec4_size));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}