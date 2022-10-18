#pragma once

#include "bounding_box.h"

class AABB : public BoundingBox {
public:
	AABB();
	AABB(const glm::vec3& pos, const glm::vec3& front, float width, float height, float depth);
};