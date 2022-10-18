#include "directional_light.h"

DirectionalLight::DirectionalLight(std::string& uniform_name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& direction) :
	Light(uniform_name, ambient, diffuse, specular) {
	this->direction = direction;
}

// setters
void DirectionalLight::set_direction(const glm::vec3& direction) {
	this->direction = direction;
}

// getters
glm::vec3 DirectionalLight::get_direction() const {
	return this->direction;
}