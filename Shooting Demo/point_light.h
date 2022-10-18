#pragma once

#include "light.h"

class PointLight :public Light {
private:
	glm::vec3 pos;
	float constant, linear, quadratic;

public:
	PointLight(std::string& uniform_name, glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& pos, float constant, float linear, float quadratic);

	// setters
	void set_pos(const glm::vec3& pos);
	void set_constant(float constant);
	void set_linear(float linear);
	void set_quadratic(float quadratic);

	// getters
	glm::vec3 get_pos() const;
	float get_constant() const;
	float get_linear() const;
	float get_quadratic() const;
};