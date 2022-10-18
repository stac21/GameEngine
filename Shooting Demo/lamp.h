#pragma once

#include "static_entity.h"
#include "aabb.h"
#include "ray_cast.h"

class Lamp :public StaticEntity {
private:
	AABB aabb;

public:
	Lamp(const glm::vec3& pos, const Model& model, AABB aabb);
	Lamp(const glm::vec3& pos, const glm::vec3& scale, const Model& model, AABB aabb);
	Lamp(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle, const Model& model, AABB aabb);
	Lamp(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle, const Model& model, AABB aabb);

	bool checkCollision(const RayCast& ray);
};