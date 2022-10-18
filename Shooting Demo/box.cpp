#include "box.h"

Box::Box()
	: center(), depth_x(0.0f), depth_y(0.0f), depth_z(0.0f) {}

Box::Box(glm::vec3 center, float depth_x, float depth_y, float depth_z)
	: center(center), depth_x(depth_x), depth_y(depth_y), depth_z(depth_z) {}

bool Box::isPointInsideBox(glm::vec3 point) {
	return (point.x >= this->min_x && point.x <= this->max_x &&
		point.y >= this->min_y && point.y <= this->max_y &&
		point.z >= this->min_z && point.z <= this->max_z);
}

// setters
void Box::set_center(float x, float y, float z) {
	this->center.x = x;
	this->center.y = y;
	this->center.z = z;
}

void Box::set_depth_x(float depth_x) {
	this->depth_x = depth_x;
}

void Box::set_depth_y(float depth_y) {
	this->depth_y = depth_y;
}

void Box::set_depth_z(float depth_z) {
	this->depth_z = depth_z;
}

// getters
glm::vec3 Box::get_center() const {
	return this->center;
}

float Box::get_depth_x() const {
	return this->depth_x;
}

float Box::get_depth_y() const {
	return this->depth_y;
}

float Box::get_depth_z() const {
	return this->depth_z;
}