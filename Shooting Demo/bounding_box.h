#pragma once

#include <glm/glm.hpp>
#include "ray_cast.h"
#include <iostream>

class BoundingBox {
protected:
	static const glm::vec3 up;
	glm::vec3 pos;
	glm::vec3 u, v, w;
	float width, height, depth;

	BoundingBox();
	BoundingBox(const glm::vec3& pos, const glm::vec3& front, float width, float height, float depth);
	virtual ~BoundingBox();

	bool is_point_in_box(const glm::vec3& point);

public:
	void update(const glm::vec3& pos);
	void update(const glm::vec3& pos, const glm::vec3& scale);

	bool check_intersect(RayCast ray);

	// getters
	glm::vec3 get_pos() const;
};