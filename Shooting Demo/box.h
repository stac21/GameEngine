#pragma once

#include <glm/glm.hpp>

// used for axis-aligned bounding box
class Box {
private:
	glm::vec3 center;
	float depth_x, depth_y, depth_z;
	float min_x, max_x, min_y, max_y, min_z, max_z;

public:
	Box();
	Box(glm::vec3 center, float depth_x, float depth_y, float depth_z);

	// true if the given point is inside of the box, false otherwise
	bool isPointInsideBox(glm::vec3 point);

	// setters
	void set_center(float x, float y, float z);
	void set_depth_x(float depth_x);
	void set_depth_y(float depth_y);
	void set_depth_z(float depth_z);

	// getters
	glm::vec3 get_center() const;
	float get_depth_x() const;
	float get_depth_y() const;
	float get_depth_z() const;
};