#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Light {
protected:
	std::string uniform_name;
	glm::vec3 ambient, diffuse, specular;

	Light(std::string& uniform_name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
	virtual ~Light();
public:
	// setters
	void set_uniform_name(std::string& uniform_name);
	void set_ambient(const glm::vec3& ambient);
	void set_diffuse(const glm::vec3& diffuse);
	void set_specular(const glm::vec3& specular);

	// getters
	std::string get_uniform_name() const;
	glm::vec3 get_ambient() const;
	glm::vec3 get_diffuse() const;
	glm::vec3 get_specular() const;
};