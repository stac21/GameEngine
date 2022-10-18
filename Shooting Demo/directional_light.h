#pragma once

#include "light.h"

class DirectionalLight :public Light {
private:
	glm::vec3 direction;

public:
	DirectionalLight(std::string& uniform_name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& direction);

	// setters
	void set_direction(const glm::vec3& direction);

	// getters
	glm::vec3 get_direction() const;
};