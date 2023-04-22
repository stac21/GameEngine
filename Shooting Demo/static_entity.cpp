#include "static_entity.h"

StaticEntity::StaticEntity(const glm::vec3& pos, const Model& model) :
	Entity(pos, model) {}

StaticEntity::StaticEntity(const glm::vec3& pos, const glm::vec3& scale, const Model& model) :
	Entity(pos, scale, model) {}

StaticEntity::StaticEntity(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle, const Model& model) :
	Entity(pos, rotation_axis, rotation_angle, model) {}

StaticEntity::StaticEntity(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle, const Model& model) :
	Entity(pos, scale, rotation_axis, rotation_angle, model) {}

void StaticEntity::draw(const Shader& shader) {
	shader.set_mat4("model", this->model_matrix);

	this->model.draw(shader);
}