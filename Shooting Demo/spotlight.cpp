#include "spotlight.h"

SpotLight::SpotLight(std::string& uniform_name, glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& pos, const glm::vec3& direction,
	float cut_off, float outer_cut_off, float constant, float linear, float quadratic) :
	Light(uniform_name, ambient, diffuse, specular) {
	this->pos = pos;
	this->direction = direction;
	this->cut_off = cut_off;
	this->outer_cut_off = outer_cut_off;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void SpotLight::print_values() {
	std::cout << "uniform_name: " << this->uniform_name << '\n';
	std::cout << "pos: " << this->pos.x << " " << this->pos.y << " " << this->pos.z << '\n';
	std::cout << "direction: " << this->direction.x << " " << this->direction.y << " " << this->direction.z << '\n';
	std::cout << "cutOff: " << this->cut_off << '\n';
	std::cout << "outerCutOff: " << this->outer_cut_off << '\n';
	std::cout << "constant: " << this->constant << '\n';
	std::cout << "linear: " << this->linear << '\n';
	std::cout << "quadratic: " << this->quadratic << '\n';
}

// setters
void SpotLight::set_pos(const glm::vec3& pos) {
	this->pos = pos;
}

void SpotLight::set_direction(const glm::vec3& direction) {
	this->direction = direction;
}

void SpotLight::set_cut_off(float cut_off) {
	this->cut_off = cut_off;
}

void SpotLight::set_outer_cut_off(float outer_cut_off) {
	this->outer_cut_off = outer_cut_off;
}

void SpotLight::set_constant(float constant) {
	this->constant = constant;
}

void SpotLight::set_linear(float linear) {
	this->linear = linear;
}

void SpotLight::set_quadratic(float quadratic) {
	this->quadratic = quadratic;
}

// getters
glm::vec3 SpotLight::get_pos() const {
	return this->pos;
}

glm::vec3 SpotLight::get_direction() const {
	return this->direction;
}

float SpotLight::get_cut_off() const {
	return this->cut_off;
}

float SpotLight::get_outer_cut_off() const {
	return this->outer_cut_off;
}

float SpotLight::get_constant() const {
	return this->constant;
}

float SpotLight::get_linear() const {
	return this->linear;
}

float SpotLight::get_quadratic() const {
	return this->quadratic;
}