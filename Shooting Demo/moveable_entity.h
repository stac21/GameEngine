#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "entity.h"

class MoveableEntity :public Entity {
private:
	void draw(const Shader& shader);

public:
	MoveableEntity(const glm::vec3& pos, const Model& model);
	MoveableEntity(const glm::vec3& pos, const glm::vec3& scale, const Model& model);
	MoveableEntity(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle, const Model& model);
	MoveableEntity(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotaton_axis, float rotation_angle, const Model& model);

	void update(const Shader& shader, float x_pos, float y_pos, float z_pos);
	void update(const Shader& shader, float x_pos, float y_pos, float z_pos, const glm::vec3& scale);
	void update(const Shader& shader, float x_pos, float y_pos, float z_pos, const glm::vec3& rotation_axis, float rotation_angle);
	void update(const Shader& shader, float x_pos, float y_pos, float z_pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle);
};