#include "point_light.h"

PointLight::PointLight(std::string& uniform_name, glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& pos, float constant, float linear,
	float quadratic) :
	Light(uniform_name, ambient, diffuse, specular) {
	this->pos = pos;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

// setters
void PointLight::set_pos(const glm::vec3& pos) {
	this->pos = pos;
}

void PointLight::set_constant(float constant) {
	this->constant = constant;
}

void PointLight::set_linear(float linear) {
	this->linear = linear;
}

void PointLight::set_quadratic(float quadratic) {
	this->quadratic = quadratic;
}

// getters
glm::vec3 PointLight::get_pos() const {
	return this->pos;
}

float PointLight::get_constant() const {
	return this->constant;
}

float PointLight::get_linear() const {
	return this->linear;
}

float PointLight::get_quadratic() const {
	return this->quadratic;
}

