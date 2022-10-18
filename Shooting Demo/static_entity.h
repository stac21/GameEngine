#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "entity.h"

class StaticEntity :public Entity {
public:
	StaticEntity(const glm::vec3& pos, const Model& model);
	StaticEntity(const glm::vec3& pos, const glm::vec3& scale, const Model& model);
	StaticEntity(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle, const Model& model);
	StaticEntity(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle, const Model& model);

	void draw(const Shader& shader);
};