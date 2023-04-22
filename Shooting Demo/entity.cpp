#include "entity.h"

Entity::Entity(const glm::vec3& pos, const Model& model) :
	model(model), pos(pos) {
	// apply any necessary transformations to the model matrix
	this->update_model_matrix(false, false);
}

Entity::Entity(const glm::vec3& pos, const glm::vec3& scale, const Model& model) :
	model(model), pos(pos), scale(scale) {
	// apply any necessary transformations to the model matrix
	this->update_model_matrix(true, false);
}

Entity::Entity(const glm::vec3& pos, const glm::vec3& rotation_axis, float rotation_angle, const Model& model) :
	model(model), pos(pos), scale(scale) {
	// apply any necessary transformations to the model matrix
	this->update_model_matrix(false, true);
}

Entity::Entity(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation_axis, float rotation_angle, const Model& model) :
	model(model), pos(pos), scale(scale), rotation_axis(rotation_axis), rotation_angle(rotation_angle) {
	// apply any necessary transformations to the model matrix
	this->update_model_matrix(true, true);
}

Entity::~Entity() {}

void Entity::update_model_matrix(bool should_scale, bool should_rotate) {
	this->model_matrix = glm::mat4();
	this->model_matrix = glm::translate(this->model_matrix, this->pos);
	if (should_rotate) {
		this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation_angle), this->rotation_axis);
	}
	if (should_scale) {
		this->model_matrix = glm::scale(this->model_matrix, this->scale);
	}
}

// setters
void Entity::set_model(const Model& model) {
	this->model = model;
}

// getters
glm::vec3 Entity::get_pos() const {
	return this->pos;
}

float Entity::get_x_pos() const {
	return this->pos.x;
}

float Entity::get_y_pos() const {
	return this->pos.y;
}

float Entity::get_z_pos() const {
	return this->pos.z;
}

glm::vec3 Entity::get_scale() const {
	return this->scale;
}

glm::vec3 Entity::get_rotation_axis() const {
	return this->rotation_axis;
}

float Entity::get_rotation_angle() const {
	return this->rotation_angle;
}

glm::mat4 Entity::get_model_matrix() const {
	return this->model_matrix;
}