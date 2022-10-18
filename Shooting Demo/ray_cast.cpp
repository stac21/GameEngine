#include "ray_cast.h"

RayCast::RayCast()
	: start_point(glm::vec3(0.0f)), direction(glm::vec3(0.0f)) {
}
RayCast::RayCast(const glm::vec3& start_point, const glm::vec3& direction)
	: start_point(start_point), direction(direction) {}

// setters
void RayCast::set_start_point(const glm::vec3& start_point) {
	this->start_point = start_point;

	//this->start_point = glm::normalize(this->start_point);
}

void RayCast::set_direction(const glm::vec3& direction) {
	this->direction = glm::normalize(direction);
}

// getters
glm::vec3 RayCast::get_start_point() const {
	return this->start_point;
}

glm::vec3 RayCast::get_direction() const {
	return this->direction;
}

glm::vec3 RayCast::get_end_point(float scalar) const {
	return this->start_point + this->direction * scalar;
}