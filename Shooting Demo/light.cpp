#include "light.h"

Light::Light(const std::string& uniform_name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) {
	this->uniform_name = uniform_name;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

Light::~Light() {}

// getters
std::string Light::get_uniform_name() const {
	return this->uniform_name;
}

glm::vec3 Light::get_ambient() const {
	return this->ambient;
}

glm::vec3 Light::get_diffuse() const {
	return this->diffuse;
}

glm::vec3 Light::get_specular() const {
	return this->specular;
}

bool Light::is_on() const {
	return this->activated;
}

// setters
void Light::set_uniform_name(std::string& uniform_name) {
	this->uniform_name = uniform_name;
}

void Light::set_ambient(const glm::vec3& ambient) {
	this->ambient = ambient;
}

void Light::set_diffuse(const glm::vec3& diffuse) {
	this->diffuse = diffuse;
}

void Light::set_specular(const glm::vec3& specular) {
	this->specular = specular;
}

void Light::toggle() {
	this->activated = !this->activated;
}

void Light::turn_on() {
	this->activated = true;
}

void Light::turn_off() {
	this->activated = false;
}

