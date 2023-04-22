#pragma once

#include <iostream>
#include <glm/glm.hpp>

std::ostream& operator<<(std::ostream& out, const glm::vec3& vec) {
	out << vec.x << ", " << vec.y << ", " << vec.z << "\n";

	return out;
}