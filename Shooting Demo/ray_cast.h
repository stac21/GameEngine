#pragma once

#include <glm/glm.hpp>

class RayCast {
private:
	glm::vec3 start_point;
	glm::vec3 direction;

public:
	RayCast();
	RayCast(const glm::vec3& start_point, const glm::vec3& direction);

	// setters
	void set_start_point(const glm::vec3& start_point);
	void set_direction(const glm::vec3& direction);

	// getters
	glm::vec3 get_start_point() const;
	glm::vec3 get_direction() const;
	glm::vec3 get_end_point(float scalar) const;
};