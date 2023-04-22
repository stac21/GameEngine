#include "lamp.h"

Lamp::Lamp(const glm::vec3& pos, const Model& model, AABB aabb)
	: StaticEntity(pos, model), aabb(aabb) {
}

Lamp::Lamp(const glm::vec3& pos, const glm::vec3& scale, const Model& model, AABB aabb)
	: StaticEntity(pos, scale, model), aabb(aabb) {
}

Lamp::Lamp(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle, const Model& model, AABB aabb)
	: StaticEntity(pos, rotation_axis, rotation_angle, model), aabb(aabb) {
}

Lamp::Lamp(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle, const Model& model, AABB aabb)
	: StaticEntity(pos, scale, rotation_axis, rotation_angle, model), aabb(aabb) {
}

bool Lamp::checkCollision(const RayCast& ray) {
	return this->aabb.check_intersect(ray);
}