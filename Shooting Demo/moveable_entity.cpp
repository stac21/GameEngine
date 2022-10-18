#include "moveable_entity.h"

MoveableEntity::MoveableEntity(const glm::vec3& pos, const Model& model) :
	Entity(pos, model) {}

MoveableEntity::MoveableEntity(const glm::vec3& pos, const glm::vec3& scale, const Model& model) :
	Entity(pos, scale, model) {}

MoveableEntity::MoveableEntity(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle, const Model& model) :
	Entity(pos, rotation_axis, rotation_angle, model) {}

MoveableEntity::MoveableEntity(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle, const Model& model) :
	Entity(pos, scale, rotation_axis, rotation_angle, model) {}

void MoveableEntity::draw(const Shader& shader) {
	shader.set_mat4("model", this->model_matrix);

	this->model.draw(shader);
}

void MoveableEntity::update(const Shader& shader, float x_pos, float y_pos, float z_pos) {
	this->pos.x = x_pos;
	this->pos.y = y_pos;
	this->pos.z = z_pos;

	// data has been changed, so update the model matrix
	update_model_matrix(false, false);

	this->draw(shader);
}

void MoveableEntity::update(const Shader& shader, float x_pos, float y_pos, float z_pos, const glm::vec3& scale) {
	this->pos.x = x_pos;
	this->pos.y = y_pos;
	this->pos.z = z_pos;
	this->scale = scale;

	// date has been changed, so update the model matrix
	update_model_matrix(true, false);

	this->draw(shader);
}

void MoveableEntity::update(const Shader& shader, float x_pos, float y_pos, float z_pos, const glm::vec3& rotation_axis, float rotation_angle) {
	this->pos.x = x_pos;
	this->pos.y = y_pos;
	this->pos.z = z_pos;
	this->rotation_axis = rotation_axis;
	this->rotation_angle = rotation_angle;

	// date has been changed, so update the model matrix
	update_model_matrix(false, true);

	this->draw(shader);
}

void MoveableEntity::update(const Shader& shader, float x_pos, float y_pos, float z_pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle) {
	this->pos.x = x_pos;
	this->pos.y = y_pos;
	this->pos.z = z_pos;
	this->scale = scale;
	this->rotation_axis = rotation_axis;
	this->rotation_angle = rotation_angle;

	// date has been changed, so update the model matrix
	update_model_matrix(true, true);

	this->draw(shader);
}