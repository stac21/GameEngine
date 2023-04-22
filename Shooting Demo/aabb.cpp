#include "aabb.h"

AABB::AABB(const glm::vec3& pos, const glm::vec3& front, float width, float height, float depth) :
	BoundingBox(pos, front, width, height, depth) {}