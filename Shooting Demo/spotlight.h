#pragma once

#include "light.h"
#include <iostream>

class SpotLight :public Light {
private:
	glm::vec3 pos, direction;
	float cut_off, outer_cut_off, constant, linear, quadratic;

public:
	SpotLight(std::string& uniform_name, glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& pos, const glm::vec3& direction,
		float cut_off, float outer_cut_off, float constant, float linear, float quadratic);

	// setters
	void set_pos(const glm::vec3& pos);
	void set_direction(const glm::vec3& direction);
	void set_cut_off(float cut_off);
	void set_outer_cut_off(float outer_cut_off);
	void set_constant(float constant);
	void set_linear(float linear);
	void set_quadratic(float quadratic);

	// getters
	glm::vec3 get_pos() const;
	glm::vec3 get_direction() const;
	float get_cut_off() const;
	float get_outer_cut_off() const;
	float get_constant() const;
	float get_linear() const;
	float get_quadratic() const;

	void print_values();
};